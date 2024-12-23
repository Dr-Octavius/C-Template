#ifndef DATATABLE_H
#define DATATABLE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

class DataTable {
private:
    std::vector<std::unordered_map<std::string, std::string>> rows; // vector(a: 1, b: 2, c: 3)

public:
    DataTable() = default;
    // Function to add a row to the table
    void addRow(const std::unordered_map<std::string, std::string>& newRow);
    void setRows(const std::vector<std::unordered_map<std::string, std::string>>& newRows);
    void clearRows();

    void insertCrossProduct(const std::unordered_map<std::string, std::vector<std::string>>& synonyms);
    void removeRowsWithoutSubTuple(const std::unordered_map<std::string, std::string>& subTuple); // No need

    void retainRowsWithSubTuples(const std::vector<std::unordered_map<std::string, std::string>>& subTuples);
    void merge(const std::vector<std::unordered_map<std::string, std::string>>& subTuples); // Join 2 tables

    // Display the contents of the table
    void displayTable();

    // Getters
    std::unordered_set<std::string> getColumnValues(const std::string& columnHeader);
    std::vector<std::vector<std::string>> getMultipleColumnValues(const std::vector<std::string>& columnHeaders);

    std::vector<std::string> getColumnHeaders();

    // TODO: Remove this after refactoring intermediate results to DataTable type
    std::vector<std::unordered_map<std::string, std::string>> getRows();

    // Helpers
    bool isEmpty();
    std::unordered_set<std::string> intersect(std::unordered_set<std::string> set1, std::unordered_set<std::string> set2);
};

#endif