#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set> 
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

void reportError(int errorType, const std::string& message, int line, std::ofstream& outputFile) {
    static std::set<int> reportedErrors; // 使用 std::set 来记录已报告的错误类型
    if (reportedErrors.find(errorType) == reportedErrors.end()) {
        std::string errorTypeName;
        switch (errorType) {
            case 1: errorTypeName = "Undefined Variable"; break;
            case 2: errorTypeName = "Undefined Function"; break;
            case 3: errorTypeName = "Variable Redefinition"; break;
            case 4: errorTypeName = "Function Redefinition"; break;
            case 5: errorTypeName = "Type Mismatch in Assignment"; break;
            case 6: errorTypeName = "Type Mismatch in Operation"; break;
            case 7: errorTypeName = "Return Type Mismatch"; break;
            case 8: errorTypeName = "Parameter Mismatch"; break;
            case 9: errorTypeName = "Array Access on Non-Array Variable"; break;
            case 10: errorTypeName = "Function Call on Non-Function Variable"; break;
            case 11: errorTypeName = "Non-Integer Array Index"; break;
            default: errorTypeName = "Unknown Error"; break;
        }

        outputFile << "Error type " << errorType << " at Line " << line << ": " << errorTypeName << " - " << message << std::endl;
        reportedErrors.insert(errorType);
    }
}

bool isValidArrayIndex(const std::string& indexType) {
    return indexType == "int";
}

bool isFloatType(const std::string& type) {
    return type == "float";
}

bool isValidArrayType(const VariableInfo& varInfo1, const VariableInfo& varInfo2) {
    return varInfo1.type == varInfo2.type && varInfo1.arrayDimensions == varInfo2.arrayDimensions;
}

bool isValidFunctionReturnType(const std::string& returnType) {
    return isValidArrayIndex(returnType) || isFloatType(returnType);
}


bool isFunctionDefined(const std::string& functionName) {
    return functions.find(functionName) != functions.end();
}

bool isVariableDefined(const std::string& variableName) {
    return globalVariables.find(variableName) != globalVariables.end();
}

bool checkVariableUsage(const std::string& variableName, const std::string& expectedType, int line, std::ofstream& outputFile) {
    if (!isVariableDefined(variableName)) {
        reportError(1, "Undefined variable \"" + variableName + "\".", line, outputFile);
        return false;
    }

    VariableInfo varInfo = globalVariables[variableName];
    if (varInfo.type != expectedType) {
        reportError(5, "Variable '" + variableName + "' has type '" + varInfo.type + "', expected '" + expectedType + "'.", line, outputFile);
        return false;
    }
    return true;
}

bool checkFunctionUsage(const std::string& functionName, const std::string& expectedReturnType, const std::vector<std::string>& argTypes, int line, std::ofstream& outputFile) {
    if (!isFunctionDefined(functionName)) {
        reportError(2, "Undefined function \"" + functionName + "\".", line, outputFile);
        return false;
    }

    FunctionInfo funcInfo = functions[functionName];
    if (funcInfo.returnType != expectedReturnType) {
        reportError(7, "Function '" + functionName + "' has return type '" + funcInfo.returnType + "', expected '" + expectedReturnType + "'.", line, outputFile);
        return false;
    }

    if (funcInfo.paramTypes != argTypes) {
        reportError(8, "Function '" + functionName + "' parameter types do not match.", line, outputFile);
        return false;
    }

    return true;
}

bool checkArrayAccess(const std::string& variableName, const std::string& indexType, int line, std::ofstream& outputFile) {
    if (!isVariableDefined(variableName)) {
        reportError(1, "Undefined variable \"" + variableName + "\".", line, outputFile);
        return false;
    }

    VariableInfo varInfo = globalVariables[variableName];
    if (varInfo.arrayDimensions == 0) {
        reportError(9, "Variable '" + variableName + "' is not an array.", line, outputFile);
        return false;
    }

    if (!isValidArrayIndex(indexType)) {
        reportError(11, "Array index for variable '" + variableName + "' must be of integer type.", line, outputFile);
        return false;
    }

    return true;
}
void defineVariable(const std::string& name, const std::string& type, int dimensions, int line, std::ofstream& outputFile) {
    if (globalVariables.find(name) != globalVariables.end()) {
        reportError(3, "Variable \"" + name + "\" already defined.", line, outputFile);
    } else {
        globalVariables[name] = {type, dimensions};
    }
}

void defineFunction(const std::string& name, const std::string& returnType, const std::vector<std::string>& paramTypes, int line, std::ofstream& outputFile) {
    if (functions.find(name) != functions.end()) {
        reportError(4, "Function \"" + name + "\" already defined.", line, outputFile);
    } else {
        functions[name] = {returnType, paramTypes};
    }
}
int main(int argc, char* argv[]) {
    std::string inputFileName, outputFileName;

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


         if (line.find("int ") == 0 || line.find("float ") == 0) {
		    std::string type = line.substr(0, line.find(" "));
		    std::string name = line.substr(line.find(" ") + 1, line.find(";") - line.find(" ") - 1);
		    int dimensions = 0; // 添加逻辑来确定维度
		    defineVariable(name, type, dimensions, lineCount, outputFile);
		}
		
		// 示例：定义函数
		if (line.find("int func(") == 0 || line.find("float func(") == 0) {
		    std::string returnType = line.substr(0, line.find(" "));
		    std::string name = line.substr(line.find("func(") + 5, line.find(")") - line.find("func(") - 5);
		    std::vector<std::string> paramTypes; // 添加逻辑来解析参数类型
		    defineFunction(name, returnType, paramTypes, lineCount, outputFile);
		}
        // 在这里进行语法分析和错误检查，并将结果写入输出文件
        // 例如：
        // 如果发现未定义的变量，输出错误信息
        if (line.find(" = ") != std::string::npos) {
            std::string variableName = line.substr(0, line.find(" = "));
            if (!isVariableDefined(variableName)) {
                reportError(1, "Undefined variable \"" + variableName + "\".", lineCount, outputFile);
            }
        }

        // 如果发现未定义的函数，输出错误信息
        if (line.find("(") != std::string::npos && line.find(")") != std::string::npos) {
            std::string functionName = line.substr(0, line.find("("));
            if (!isFunctionDefined(functionName)) {
                reportError(2, "Undefined function \"" + functionName + "\".", lineCount, outputFile);
            }
        }

        // 检查其他错误类型的逻辑可以根据需要添加，确保使用相应的错误类型序号

        // 样例 3：检查变量重定义错误
        if (line.find("int ") != std::string::npos) {
            std::string variableName = line.substr(line.find("int ") + 4);
            variableName = variableName.substr(0, variableName.find(";"));
            if (isVariableDefined(variableName)) {
                reportError(3, "Redefined variable \"" + variableName + "\".", lineCount, outputFile);
            }
        }

        // 样例 4：检查函数重定义错误
        if (line.find("int func(") != std::string::npos || line.find("float func(") != std::string::npos) {
            std::string functionName = line.substr(line.find("func("));
            functionName = functionName.substr(0, functionName.find(")"));
            if (isFunctionDefined(functionName)) {
                reportError(4, "Redefined function \"" + functionName + "\".", lineCount, outputFile);
            }
        }

        // 样例 5：检查类型不匹配的赋值错误
        if (line.find(" = ") != std::string::npos) {
            std::string assignment = line.substr(line.find(" = ") + 3);
            std::string variableName = line.substr(0, line.find(" = "));
            variableName = variableName.substr(variableName.find_last_of(" \t") + 1);
            std::string variableType = globalVariables[variableName].type;
            if (variableType == "int" && assignment.find_first_of("fF.") != std::string::npos) {
                reportError(5, "Type mismatched for assignment.", lineCount, outputFile);
            }
        }

        // 样例 6：检查操作数类型不匹配的错误
        if (line.find("+") != std::string::npos || line.find("-") != std::string::npos || line.find("*") != std::string::npos || line.find("/") != std::string::npos) {
            std::string variableName = line.substr(line.find("+") - 1, 3);
            variableName = variableName.substr(variableName.find_last_of(" \t") + 1);
            std::string variableType = globalVariables[variableName].type;
            if (variableType == "int" && line.find_first_of("fF.") != std::string::npos) {
                reportError(6, "Type mismatched for operands.", lineCount, outputFile);
            }
        }

        // 样例 7：检查返回类型不匹配的错误
        if (line.find("return") != std::string::npos) {
            std::string returnType = line.substr(line.find("return") + 6);
            returnType = returnType.substr(0, returnType.find(";"));
            if (returnType != "int" && returnType != "float") {
                reportError(7, "Type mismatched for return.", lineCount, outputFile);
            }
        }

        // 样例 8：检查函数调用参数数量和类型不匹配的错误
        if (line.find("(") != std::string::npos && line.find(")") != std::string::npos) {
            std::string functionName = line.substr(0, line.find("("));
            std::string argList = line.substr(line.find("(") + 1, line.find(")") - line.find("(") - 1);
            if (isFunctionDefined(functionName)) {
                FunctionInfo funcInfo = functions[functionName];
                std::vector<std::string> argTypes;
                size_t startPos = 0;
                size_t commaPos = argList.find(",");
                while (commaPos != std::string::npos) {
                    std::string arg = argList.substr(startPos, commaPos - startPos);
                    argTypes.push_back(arg);
                    startPos = commaPos + 1;
                    commaPos = argList.find(",", startPos);
                }
                std::string lastArg = argList.substr(startPos);
                argTypes.push_back(lastArg);
                if (funcInfo.paramTypes != argTypes) {
                    reportError(8, "Function \"" + functionName + "\" is not applicable for arguments \"" + argList + "\".", lineCount, outputFile);
                }
            }
        }

        // 样例 9：检查非数组型变量使用数组访问符的错误
        if (line.find("[") != std::string::npos) {
            std::string variableName = line.substr(0, line.find("["));
            if (isVariableDefined(variableName) && globalVariables[variableName].arrayDimensions == 0) {
                reportError(9, "\"" + variableName + "\" is not an array.", lineCount, outputFile);
            }
        }

        // 样例 10：检查非函数型变量使用函数调用操作符的错误
        if (line.find("(") != std::string::npos && line.find(")") != std::string::npos && line.find("=") == std::string::npos) {
            std::string variableName = line.substr(0, line.find("("));
            if (isVariableDefined(variableName) && !isFunctionDefined(variableName)) {
                reportError(10, "\"" + variableName + "\" is not a function.", lineCount, outputFile);
            }
        }

        // 样例 11：检查数组访问操作符中出现非整数的错误
        if (line.find("[") != std::string::npos) {
            std::string index = line.substr(line.find("[") + 1, line.find("]") - line.find("[") - 1);
            if (!isValidArrayIndex(index)) {
                reportError(11, "\"" + index + "\" is not an integer.", lineCount, outputFile);
            }
        }
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
