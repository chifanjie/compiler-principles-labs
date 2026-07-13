#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

struct VariableInfo {
    std::string type;
    int arrayDimensions;
};

struct FunctionInfo {
    std::string returnType;
    std::vector<std::string> paramTypes;
};

std::unordered_map<std::string, VariableInfo> globalVariables;
std::unordered_map<std::string, FunctionInfo> functions;

void reportError(const std::string& errorType, const std::string& message, int line, std::ofstream& outputFile) {
    outputFile << "Error type [" << errorType << "] at Line " << line << ": " << message << std::endl;
}

bool isIntegerType(const std::string& type) {
    return type == "int";
}

bool isFloatType(const std::string& type) {
    return type == "float";
}

bool isValidArrayType(const VariableInfo& varInfo1, const VariableInfo& varInfo2) {
    return varInfo1.type == varInfo2.type && varInfo1.arrayDimensions == varInfo2.arrayDimensions;
}

bool isValidFunctionReturnType(const std::string& returnType) {
    return isIntegerType(returnType) || isFloatType(returnType);
}

bool isFunctionDefined(const std::string& functionName) {
    return functions.find(functionName) != functions.end();
}

bool isVariableDefined(const std::string& variableName) {
    return globalVariables.find(variableName) != globalVariables.end();
}

bool checkVariableUsage(const std::string& variableName, const std::string& expectedType, int line, std::ofstream& outputFile) {
    if (!isVariableDefined(variableName)) {
        reportError("Undefined Variable", "Variable '" + variableName + "' is undefined.", line, outputFile);
        return false;
    }

    VariableInfo varInfo = globalVariables[variableName];
    if (varInfo.type != expectedType) {
        reportError("Type Mismatch", "Variable '" + variableName + "' has type '" + varInfo.type + "', expected '" + expectedType + "'.", line, outputFile);
        return false;
    }
    return true;
}

bool checkFunctionUsage(const std::string& functionName, const std::string& expectedReturnType, const std::vector<std::string>& argTypes, int line, std::ofstream& outputFile) {
    if (!isFunctionDefined(functionName)) {
        reportError("Undefined Function", "Function '" + functionName + "' is undefined.", line, outputFile);
        return false;
    }

    FunctionInfo funcInfo = functions[functionName];
    if (funcInfo.returnType != expectedReturnType) {
        reportError("Type Mismatch", "Function '" + functionName + "' has return type '" + funcInfo.returnType + "', expected '" + expectedReturnType + "'.", line, outputFile);
        return false;
    }

    if (funcInfo.paramTypes != argTypes) {
        reportError("Parameter Mismatch", "Function '" + functionName + "' parameter types do not match.", line, outputFile);
        return false;
    }

    return true;
}

bool checkArrayAccess(const std::string& variableName, const std::string& indexType, int line, std::ofstream& outputFile) {
    if (!isVariableDefined(variableName)) {
        reportError("Undefined Variable", "Variable '" + variableName + "' is undefined.", line, outputFile);
        return false;
    }

    VariableInfo varInfo = globalVariables[variableName];
    if (varInfo.arrayDimensions == 0) {
        reportError("Array Access", "Variable '" + variableName + "' is not an array.", line, outputFile);
        return false;
    }

    if (!isIntegerType(indexType)) {
        reportError("Array Index Type Mismatch", "Array index for variable '" + variableName + "' must be of integer type.", line, outputFile);
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    std::string inputFileName, outputFileName;

    // 如果未提供命令行参数，则手动输入输入和输出文件名
    if (argc != 3) {
        std::cout << "Enter input file name: ";
        std::cin >> inputFileName;

        std::cout << "Enter output file name: ";
        std::cin >> outputFileName;
    } else {
        inputFileName = argv[1];
        outputFileName = argv[2];
    }

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Cannot open input file '" << inputFileName << "'" << std::endl;
        return 1;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Cannot open output file '" << outputFileName << "'" << std::endl;
        return 1;
    }

    std::string line;
    int lineCount = 0;
    while (std::getline(inputFile, line)) {
        ++lineCount;

        // 在这里进行语法分析和错误检查，并将结果写入输出文件
        // 例如：
        // 如果发现未定义的变量，输出错误信息
        if (line.find(" = ") != std::string::npos) {
            std::string variableName = line.substr(0, line.find(" = "));
            if (!isVariableDefined(variableName)) {
                reportError("Undefined Variable", "Undefined variable \"" + variableName + "\".", lineCount, outputFile);
            }
        }

        // 如果发现未定义的函数，输出错误信息
        if (line.find("(") != std::string::npos && line.find(")") != std::string::npos) {
            std::string functionName = line.substr(0, line.find("("));
            if (!isFunctionDefined(functionName)) {
                reportError("Undefined Function", "Undefined function \"" + functionName + "\".", lineCount, outputFile);
            }
        }

        // 其他错误检查逻辑可以根据需要添加
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
