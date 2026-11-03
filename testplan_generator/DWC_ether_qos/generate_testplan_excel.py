#!/usr/bin/env python3
"""
DWC_ether_qos TestPlan Excel Generator
Agent 7 - Excel Generation and GitHub Publishing

Generates a genuine Microsoft Excel workbook (.xlsx) with:
- TestPlan sheet (visible) with Code Generation dropdown
- MetaData sheet (veryHidden)
- Proper formatting (frozen panes, wrap text, blue headers)

Usage:
    pip install openpyxl
    python3 generate_testplan_excel.py
"""

import json
import os
import sys
from datetime import datetime, timezone, timedelta

try:
    from openpyxl import Workbook, load_workbook
    from openpyxl.styles import Font, PatternFill, Alignment, Border, Side
    from openpyxl.worksheet.datavalidation import DataValidation
    from openpyxl.utils import get_column_letter
except ImportError:
    print("ERROR: openpyxl required. Install: pip install openpyxl")
    sys.exit(1)

IP_NAME = "DWC_ether_qos"
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
META_JSON_PATH = os.path.join(SCRIPT_DIR, "meta_data.json")
TESTPLAN_JSON_PATH = os.path.join(SCRIPT_DIR, "testplan_data.json")

IST = timezone(timedelta(hours=5, minutes=30))
timestamp = datetime.now(IST).strftime("%Y%m%d_%H%M%S")
OUTPUT_FILENAME = f"{IP_NAME}_TestPlan_{timestamp}.xlsx"
OUTPUT_PATH = os.path.join(SCRIPT_DIR, OUTPUT_FILENAME)

TESTPLAN_COLUMNS = [
    "Index", "SS / Module", "Feature", "Test Case Name",
    "Speed", "Mode", "Memory Start Offset", "Memory End Offset",
    "Test Description", "Test Steps / Procedure", "Impacted Registers",
    "Validation / Acceptance Criteria", "Remarks",
    "Code Generation (Required / Not)"
]

METADATA_COLUMNS = [
    "Test Case Name", "Meta Headers", "Meta Macros", "Meta Arrays",
    "Meta Types", "Meta Test Description", "Meta Test Steps / Procedure",
    "Meta Impacted Registers", "Meta Validation / Acceptance Criteria"
]

HEADER_FILL = PatternFill(start_color="4472C4", end_color="4472C4", fill_type="solid")
HEADER_FONT = Font(name="Calibri", size=11, bold=True, color="FFFFFF")
DATA_FONT = Font(name="Calibri", size=11)
WRAP_ALIGN = Alignment(wrap_text=True, vertical="top", horizontal="left")
HDR_ALIGN = Alignment(wrap_text=True, vertical="center", horizontal="center")
BORDER = Border(
    left=Side(style="thin"), right=Side(style="thin"),
    top=Side(style="thin"), bottom=Side(style="thin")
)

def load_json(path):
    with open(path, "r", encoding="utf-8") as f:
        data = json.load(f)
    assert isinstance(data, list) and len(data) > 0
    return data

def main():
    meta = load_json(META_JSON_PATH)
    tp = load_json(TESTPLAN_JSON_PATH)

    meta_names = {m["Test Case Name"] for m in meta}
    for t in tp:
        assert t["Test Case Name"] in meta_names

    wb = Workbook()

    # TestPlan sheet
    ws = wb.active
    ws.title = "TestPlan"
    for ci, cn in enumerate(TESTPLAN_COLUMNS, 1):
        c = ws.cell(row=1, column=ci, value=cn)
        c.fill, c.font, c.alignment, c.border = HEADER_FILL, HEADER_FONT, HDR_ALIGN, BORDER

    for ri, row in enumerate(tp, 2):
        for ci, cn in enumerate(TESTPLAN_COLUMNS, 1):
            val = "" if cn == "Code Generation (Required / Not)" else row.get(cn, "")
            c = ws.cell(row=ri, column=ci, value=val)
            c.font, c.alignment, c.border = DATA_FONT, WRAP_ALIGN, BORDER

    dv = DataValidation(type="list", formula1='"Required,Not Required"', allow_blank=True, showDropDown=False)
    col_letter = get_column_letter(len(TESTPLAN_COLUMNS))
    dv.add(f"{col_letter}2:{col_letter}{len(tp)+1}")
    ws.add_data_validation(dv)
    ws.freeze_panes = "A2"

    for ci, cn in enumerate(TESTPLAN_COLUMNS, 1):
        mx = len(cn)
        for row in tp:
            v = str(row.get(cn, ""))
            mx = max(mx, len(v.split("\\n")[0][:80]))
        ws.column_dimensions[get_column_letter(ci)].width = min(max(mx+2, 12), 60)

    # MetaData sheet
    ms = wb.create_sheet(title="MetaData")
    for ci, cn in enumerate(METADATA_COLUMNS, 1):
        c = ms.cell(row=1, column=ci, value=cn)
        c.fill, c.font, c.alignment, c.border = HEADER_FILL, HEADER_FONT, HDR_ALIGN, BORDER

    for ri, row in enumerate(meta, 2):
        for ci, cn in enumerate(METADATA_COLUMNS, 1):
            c = ms.cell(row=ri, column=ci, value=row.get(cn, ""))
            c.font, c.alignment, c.border = DATA_FONT, WRAP_ALIGN, BORDER

    ms.freeze_panes = "A2"
    ms.sheet_state = "veryHidden"

    for ci, cn in enumerate(METADATA_COLUMNS, 1):
        mx = len(cn)
        for row in meta:
            v = str(row.get(cn, ""))
            mx = max(mx, len(v.split("\\n")[0][:80]))
        ms.column_dimensions[get_column_letter(ci)].width = min(max(mx+2, 12), 60)

    wb.save(OUTPUT_PATH)
    wb.close()

    # Validate
    wb2 = load_workbook(OUTPUT_PATH)
    assert "TestPlan" in wb2.sheetnames
    assert "MetaData" in wb2.sheetnames
    assert wb2["TestPlan"].max_row - 1 == len(tp)
    assert wb2["MetaData"].max_row - 1 == len(meta)
    wb2.close()

    print(f"Generated: {OUTPUT_PATH} ({os.path.getsize(OUTPUT_PATH)} bytes)")
    print(f"TestPlan rows: {len(tp)}, MetaData rows: {len(meta)}")

if __name__ == "__main__":
    main()
