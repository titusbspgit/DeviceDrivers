#!/usr/bin/env python3
"""
DWC_ether_qos TestPlan Excel Generator
Agent 7 - Excel Generation and GitHub Publishing

Generates a genuine Microsoft Excel workbook (.xlsx) with:
- TestPlan sheet (visible)
- MetaData sheet (veryHidden)
- Code Generation dropdown column
- Proper formatting (frozen panes, wrap text, blue headers)

Usage:
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
    print("ERROR: openpyxl is required. Install with: pip install openpyxl")
    sys.exit(1)

# ============================================================
# CONFIGURATION
# ============================================================
IP_NAME = "DWC_ether_qos"
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
META_JSON_PATH = os.path.join(SCRIPT_DIR, "meta_data.json")
TESTPLAN_JSON_PATH = os.path.join(SCRIPT_DIR, "testplan_data.json")

# IST Timezone (UTC+5:30)
IST = timezone(timedelta(hours=5, minutes=30))
timestamp = datetime.now(IST).strftime("%Y%m%d_%H%M%S")
OUTPUT_FILENAME = f"{IP_NAME}_TestPlan_{timestamp}.xlsx"
OUTPUT_PATH = os.path.join(SCRIPT_DIR, OUTPUT_FILENAME)

# ============================================================
# TESTPLAN SHEET COLUMNS
# ============================================================
TESTPLAN_COLUMNS = [
    "Index",
    "SS / Module",
    "Feature",
    "Test Case Name",
    "Speed",
    "Mode",
    "Memory Start Offset",
    "Memory End Offset",
    "Test Description",
    "Test Steps / Procedure",
    "Impacted Registers",
    "Validation / Acceptance Criteria",
    "Remarks",
    "Code Generation (Required / Not)"
]

# ============================================================
# METADATA SHEET COLUMNS
# ============================================================
METADATA_COLUMNS = [
    "Test Case Name",
    "Meta Headers",
    "Meta Macros",
    "Meta Arrays",
    "Meta Types",
    "Meta Test Description",
    "Meta Test Steps / Procedure",
    "Meta Impacted Registers",
    "Meta Validation / Acceptance Criteria"
]

# ============================================================
# FORMATTING CONSTANTS
# ============================================================
HEADER_FILL = PatternFill(start_color="4472C4", end_color="4472C4", fill_type="solid")
HEADER_FONT = Font(name="Calibri", size=11, bold=True, color="FFFFFF")
DATA_FONT = Font(name="Calibri", size=11)
WRAP_ALIGNMENT = Alignment(wrap_text=True, vertical="top", horizontal="left")
HEADER_ALIGNMENT = Alignment(wrap_text=True, vertical="center", horizontal="center")
THIN_BORDER = Border(
    left=Side(style="thin"),
    right=Side(style="thin"),
    top=Side(style="thin"),
    bottom=Side(style="thin")
)
MAX_COL_WIDTH = 60
MIN_COL_WIDTH = 12


def load_json(path):
    """Load and validate JSON file."""
    if not os.path.exists(path):
        print(f"ERROR: JSON file not found: {path}")
        sys.exit(1)
    with open(path, "r", encoding="utf-8") as f:
        data = json.load(f)
    if not isinstance(data, list) or len(data) == 0:
        print(f"ERROR: JSON must be a non-empty array: {path}")
        sys.exit(1)
    return data


def cross_validate(meta_json, testplan_json):
    """Verify every testcase in TESTPLAN_JSON exists in META_JSON."""
    meta_names = {item["Test Case Name"] for item in meta_json}
    for tp in testplan_json:
        tc_name = tp["Test Case Name"]
        if tc_name not in meta_names:
            print(f"CROSS-VALIDATION FAILED: '{tc_name}' in TESTPLAN but not in META")
            return False
    return True


def auto_size_columns(ws, columns, data_rows, key_map):
    """Auto-size columns with a reasonable maximum width."""
    for col_idx, col_name in enumerate(columns, 1):
        max_len = len(col_name)
        for row_data in data_rows:
            val = str(row_data.get(key_map.get(col_name, col_name), ""))
            # Use first line length for multi-line content
            first_line = val.split("\n")[0] if val else ""
            max_len = max(max_len, len(first_line))
        width = min(max(max_len + 2, MIN_COL_WIDTH), MAX_COL_WIDTH)
        ws.column_dimensions[get_column_letter(col_idx)].width = width


def create_testplan_sheet(wb, testplan_json):
    """Create and populate the TestPlan sheet."""
    ws = wb.active
    ws.title = "TestPlan"

    # Write headers
    for col_idx, col_name in enumerate(TESTPLAN_COLUMNS, 1):
        cell = ws.cell(row=1, column=col_idx, value=col_name)
        cell.fill = HEADER_FILL
        cell.font = HEADER_FONT
        cell.alignment = HEADER_ALIGNMENT
        cell.border = THIN_BORDER

    # Write data rows
    for row_idx, tp in enumerate(testplan_json, 2):
        for col_idx, col_name in enumerate(TESTPLAN_COLUMNS, 1):
            if col_name == "Code Generation (Required / Not)":
                cell = ws.cell(row=row_idx, column=col_idx, value="")
            elif col_name == "Speed":
                cell = ws.cell(row=row_idx, column=col_idx, value=tp.get("Speed", ""))
            elif col_name == "Mode":
                cell = ws.cell(row=row_idx, column=col_idx, value=tp.get("Mode", ""))
            elif col_name == "Memory Start Offset":
                cell = ws.cell(row=row_idx, column=col_idx, value=tp.get("Memory Start Offset", ""))
            elif col_name == "Memory End Offset":
                cell = ws.cell(row=row_idx, column=col_idx, value=tp.get("Memory End Offset", ""))
            else:
                cell = ws.cell(row=row_idx, column=col_idx, value=tp.get(col_name, ""))
            cell.font = DATA_FONT
            cell.alignment = WRAP_ALIGNMENT
            cell.border = THIN_BORDER

    # Add Code Generation dropdown validation
    code_gen_col = len(TESTPLAN_COLUMNS)
    dv = DataValidation(
        type="list",
        formula1='"Required,Not Required"',
        allow_blank=True,
        showDropDown=False
    )
    dv.error = "Please select Required or Not Required"
    dv.errorTitle = "Invalid Selection"
    dv.prompt = "Select Code Generation status"
    dv.promptTitle = "Code Generation"
    last_row = len(testplan_json) + 1
    col_letter = get_column_letter(code_gen_col)
    dv.add(f"{col_letter}2:{col_letter}{last_row}")
    ws.add_data_validation(dv)

    # Freeze first row
    ws.freeze_panes = "A2"

    # Auto-size columns
    for col_idx, col_name in enumerate(TESTPLAN_COLUMNS, 1):
        max_len = len(col_name)
        for tp in testplan_json:
            val = str(tp.get(col_name, ""))
            first_line = val.split("\n")[0] if val else ""
            max_len = max(max_len, len(first_line))
        width = min(max(max_len + 2, MIN_COL_WIDTH), MAX_COL_WIDTH)
        ws.column_dimensions[get_column_letter(col_idx)].width = width

    return ws


def create_metadata_sheet(wb, meta_json):
    """Create and populate the MetaData sheet (veryHidden)."""
    ws = wb.create_sheet(title="MetaData")

    # Write headers
    for col_idx, col_name in enumerate(METADATA_COLUMNS, 1):
        cell = ws.cell(row=1, column=col_idx, value=col_name)
        cell.fill = HEADER_FILL
        cell.font = HEADER_FONT
        cell.alignment = HEADER_ALIGNMENT
        cell.border = THIN_BORDER

    # Write data rows
    for row_idx, meta in enumerate(meta_json, 2):
        for col_idx, col_name in enumerate(METADATA_COLUMNS, 1):
            cell = ws.cell(row=row_idx, column=col_idx, value=meta.get(col_name, ""))
            cell.font = DATA_FONT
            cell.alignment = WRAP_ALIGNMENT
            cell.border = THIN_BORDER

    # Freeze first row
    ws.freeze_panes = "A2"

    # Auto-size columns
    for col_idx, col_name in enumerate(METADATA_COLUMNS, 1):
        max_len = len(col_name)
        for meta in meta_json:
            val = str(meta.get(col_name, ""))
            first_line = val.split("\n")[0] if val else ""
            max_len = max(max_len, len(first_line))
        width = min(max(max_len + 2, MIN_COL_WIDTH), MAX_COL_WIDTH)
        ws.column_dimensions[get_column_letter(col_idx)].width = width

    # Set sheet to veryHidden
    ws.sheet_state = "veryHidden"

    return ws


def validate_workbook(path, testplan_count, meta_count):
    """Post-save validation of the workbook."""
    checks = []

    # 1. File exists
    checks.append(("File exists", os.path.exists(path)))

    # 2. File size > 0
    checks.append(("File size > 0", os.path.getsize(path) > 0 if os.path.exists(path) else False))

    # 3-10. Workbook structure validation
    try:
        wb = load_workbook(path)
        checks.append(("Workbook reopens", True))
        checks.append(("TestPlan sheet exists", "TestPlan" in wb.sheetnames))
        checks.append(("MetaData sheet exists", "MetaData" in wb.sheetnames))

        tp_ws = wb["TestPlan"]
        tp_rows = tp_ws.max_row - 1  # Exclude header
        checks.append((f"TestPlan rows ({tp_rows}) == expected ({testplan_count})", tp_rows == testplan_count))

        md_ws = wb["MetaData"]
        md_rows = md_ws.max_row - 1  # Exclude header
        checks.append((f"MetaData rows ({md_rows}) == expected ({meta_count})", md_rows == meta_count))

        # Check Code Generation column exists
        last_col = tp_ws.max_column
        code_gen_header = tp_ws.cell(row=1, column=last_col).value
        checks.append(("Code Generation column exists", code_gen_header == "Code Generation (Required / Not)"))

        # Check data validation exists
        checks.append(("Data validation exists", len(tp_ws.data_validations.dataValidation) > 0))

        # Check Code Generation cells are blank
        all_blank = all(
            (tp_ws.cell(row=r, column=last_col).value is None or tp_ws.cell(row=r, column=last_col).value == "")
            for r in range(2, testplan_count + 2)
        )
        checks.append(("Code Generation cells blank", all_blank))

        wb.close()
    except Exception as e:
        checks.append((f"Workbook validation error: {e}", False))

    all_passed = all(result for _, result in checks)

    print("\n=== POST-SAVE VALIDATION ===")
    for check_name, result in checks:
        status = "PASS" if result else "FAIL"
        print(f"  [{status}] {check_name}")
    print(f"\n  Overall: {'PASSED' if all_passed else 'FAILED'}")

    return all_passed


def main():
    print(f"=== DWC_ether_qos TestPlan Excel Generator ===")
    print(f"Timestamp (IST): {datetime.now(IST).strftime('%Y-%m-%d %H:%M:%S')}")
    print(f"Output: {OUTPUT_FILENAME}")

    # Load JSON data
    print("\nLoading META_JSON...")
    meta_json = load_json(META_JSON_PATH)
    print(f"  Loaded {len(meta_json)} metadata entries")

    print("Loading TESTPLAN_JSON...")
    testplan_json = load_json(TESTPLAN_JSON_PATH)
    print(f"  Loaded {len(testplan_json)} testplan entries")

    # Cross-validate
    print("\nCross-validating datasets...")
    if not cross_validate(meta_json, testplan_json):
        print("FAILURE: Cross-dataset validation failed")
        sys.exit(1)
    print("  Cross-validation PASSED")

    # Create workbook
    print("\nCreating workbook...")
    wb = Workbook()

    # Create TestPlan sheet
    print("  Creating TestPlan sheet...")
    create_testplan_sheet(wb, testplan_json)
    print(f"    {len(testplan_json)} rows written")

    # Create MetaData sheet
    print("  Creating MetaData sheet (veryHidden)...")
    create_metadata_sheet(wb, meta_json)
    print(f"    {len(meta_json)} rows written")

    # Save workbook
    print(f"\nSaving workbook: {OUTPUT_FILENAME}")
    wb.save(OUTPUT_PATH)
    wb.close()
    print(f"  Saved successfully ({os.path.getsize(OUTPUT_PATH)} bytes)")

    # Validate
    validation_passed = validate_workbook(OUTPUT_PATH, len(testplan_json), len(meta_json))

    if validation_passed:
        print(f"\n=== SUCCESS ===")
        print(f"File: {OUTPUT_PATH}")
        print(f"TestPlan rows: {len(testplan_json)}")
        print(f"MetaData rows: {len(meta_json)}")
    else:
        print(f"\n=== FAILURE ===")
        sys.exit(1)

    return OUTPUT_PATH


if __name__ == "__main__":
    main()
