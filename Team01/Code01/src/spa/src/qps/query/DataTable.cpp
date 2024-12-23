#include "DataTable.h"
#include <regex>

// Function to add a row to the table
void DataTable::addRow(const std::unordered_map<std::string, std::string>& newRow) {
    rows.push_back(newRow);
}

void DataTable::setRows(const std::vector<std::unordered_map<std::string, std::string>>& newRows) {
    rows = newRows;
}

void DataTable::clearRows() {
    rows.clear();
}

std::vector<std::string> DataTable::getColumnHeaders() {
    if (this->isEmpty()) {
        return std::vector<std::string>();
    }
    auto headers = std::vector<std::string>();
    auto firstRow = rows[0];
    for (auto pair : firstRow) {
        headers.push_back(pair.first);
    }
    return headers;
}

std::vector<std::unordered_map<std::string, std::string>> DataTable::getRows() {
    return rows;
}

bool DataTable::isEmpty() {
    return rows.empty();
}

std::unordered_set<std::string> DataTable::intersect(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2) {
    std::unordered_set<std::string> result;
    for (const auto& element : set1) {
        if (set2.find(element) != set2.end()) {
            result.insert(element);
        }
    }
    return result;
}

void DataTable::insertCrossProduct(const std::unordered_map<std::string, std::vector<std::string>>& synonyms) {
    // Generate cross product
    std::vector<std::unordered_map<std::string, std::string>> crossProduct;
    for (const auto& pair : synonyms) {
        const std::string& synonymName = pair.first;
        const std::vector<std::string>& values = pair.second;

        if (crossProduct.empty()) {
            for (const auto& value : values) {
                std::unordered_map<std::string, std::string> row;
                row[synonymName] = value;
                crossProduct.push_back(row);
            }
        } else {
            std::vector<std::unordered_map<std::string, std::string>> newCrossProduct;
            for (const auto& existingRow : crossProduct) {
                for (const auto& value : values) {
                    auto newRow = existingRow;
                    newRow[synonymName] = value;
                    newCrossProduct.push_back(newRow);
                }
            }
            crossProduct = std::move(newCrossProduct);
        }
    }
    // Insert into the table
    for (const auto& row : crossProduct) {
        addRow(row);
    }
}

void DataTable::removeRowsWithoutSubTuple(const std::unordered_map<std::string, std::string>& subTuple) {
    auto it = std::remove_if(rows.begin(), rows.end(),
        [subTuple](const auto& row) {
            return !std::all_of(subTuple.begin(), subTuple.end(),
                [&row](const auto& condition) {
                    auto itColumn = row.find(condition.first);
                    return itColumn != row.end() && itColumn->second == condition.second;
                });
        });

    rows.erase(it, rows.end());
}

void DataTable::retainRowsWithSubTuples(const std::vector<std::unordered_map<std::string, std::string>>& subTuples) {
    // Safeguard: Check if all keys in any subTuple exist in columnHeaders
    auto columnHeaders = this->getColumnHeaders();
    std::vector<int> retainedIndices = std::vector<int>();
    for (const auto& subTuple : subTuples) {
        for (const auto& condition : subTuple) {
            if (std::find(columnHeaders.begin(), columnHeaders.end(), condition.first) == columnHeaders.end()) {
                // Handle the case where a key in subTuple is not a columnHeader
                std::cerr << "Warning: Key '" << condition.first << "' does not exist in the table.\n";
                return;
            }
        }
    }

    auto it = std::remove_if(rows.begin(), rows.end(),
        [subTuples](const auto& row) {
            return !std::any_of(subTuples.begin(), subTuples.end(),
                [&row](const auto& subTuple) {
                    return std::all_of(subTuple.begin(), subTuple.end(),
                        [&row](const auto& condition) {
                            auto itColumn = row.find(condition.first);
                            return itColumn != row.end() && itColumn->second == condition.second;
                        });
                });
        });

    rows.erase(it, rows.end());
}

void DataTable::displayTable() {
    // Display column headers
    auto columnHeaders = this->getColumnHeaders();
    for (const auto& header : columnHeaders) {
        std::cout << header << "\t";
    }
    std::cout << std::endl;

    // Display rows
    for (const auto& row : rows) {
        for (const auto& header : columnHeaders) {
            auto it = row.find(header);
            if (it != row.end()) {
                std::cout << it->second << "\t";
            } else {
                // Handle missing values as needed
                std::cout << "N/A\t";
            }
        }
        std::cout << std::endl;
    }
}

// Function to get the set of strings in a specific column
std::unordered_set<std::string> DataTable::getColumnValues(const std::string& columnHeader) {
    auto columnHeaders = this->getColumnHeaders();
    std::unordered_set<std::string> columnValues;

    // Find the index of the specified column header
    auto columnIndex = std::find(columnHeaders.begin(), columnHeaders.end(), columnHeader);

    // If the column header is found
    if (columnIndex != columnHeaders.end()) {
        // Iterate through rows and add values to the set
        for (const auto& row : rows) {
            auto it = row.find(columnHeader);
            if (it != row.end()) {
                columnValues.insert(it->second);
            }
        }
    }
    return columnValues;
}

std::vector<std::vector<std::string>> DataTable::getMultipleColumnValues(const std::vector<std::string>& columnHeaders) {
    std::vector<std::vector<std::string>> columnTuples;
    for (const auto& row : rows) {
        std::vector<std::string> tuple;
        for (const auto& header : columnHeaders) {
            auto it = row.find(header);
            if (it != row.end()) {
                tuple.push_back(it->second); // Append the value of the current column followed by a space
            } else {
                tuple.push_back("N/A"); // If the value is missing, append "N/A" followed by a space
            }
        }
        columnTuples.push_back(tuple);
    }

    return columnTuples;
}

void DataTable::merge(const std::vector<std::unordered_map<std::string, std::string>>& subTuples) {
    if(subTuples.empty()) {
        return;
    }
    if(this->isEmpty()) {
        this->setRows(subTuples);
        return;
    }

    auto mergedRows = std::vector<std::unordered_map<std::string, std::string>>();

    for (auto row : rows) {
        for (auto subTuple : subTuples) {
            auto mergedRow = row;
            bool valid = true;
            for (auto pair : subTuple) {
                std::string header = pair.first;
                std::string value = pair.second;
                // Check if column is shared and have different values -> Don't include row
                if (row.find(header) != row.end() && row[header] != value) {
                    valid = false;
                    break;
                }
                mergedRow[header] = value;
            }
            if (valid) {
                mergedRows.push_back(mergedRow);
            }
        }
    }
    setRows(mergedRows);
}

void test(std::unordered_set<int> s) {
    if (s.size() == 3) {
        return;
    }
    for (auto& val : s) {
        std::cout << " " << val;
    }
    std::cout << std::endl;
    s.insert(1);
    test(s);
}