#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>

using namespace std;
// ====================================== CRC8 ======================================
// CRC-8 Generator Polynomial
const uint16_t crc8_poly = 0x31;  // x^8 + x^5 + x^4 + 1
uint8_t calculateCRC8(const string& data) {
    uint8_t crc = 0;
    for (char bit : data) {
        crc ^= (bit - '0') << 7;
        for (int i = 0; i < 1; i++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ crc8_poly;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

// ====================================== CRC16 ======================================
// CRC-16 Generator Polynomial
const uint32_t crc16_poly = 0x11021;  // x^16 + x^12 + x^5 + 1
uint16_t calculateCRC16(const string& data) {
    uint16_t crc = 0;
    for (char bit : data) {
        crc ^= (bit - '0') << 15;
        for (int i = 0; i < 1; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ crc16_poly;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

// ====================================== CHECKSUM ======================================
// Function to calculate the one-byte version of checksum
uint8_t calculateChecksum(const string& data) {
    uint32_t sum = 0;
    // Iterate through the data in 8-bit blocks
    for (size_t i = 0; i < data.length(); i += 8) {
        string block = data.substr(i, 8); // Get the next 8 bits
        sum += bitset<8>(block).to_ulong(); // Convert to decimal and add to sum
    }
    uint32_t quotient = sum / 256;
    uint32_t remainder = sum % 256;
    uint32_t result = quotient + remainder;

    // If result exceeds 255, calculate new quotient and remainder
    while (result > 255) {
        quotient = result / 256;
        remainder = result % 256;
        result = quotient + remainder;
    }

    // Calculate one-byte version of internet checksum by taking 1's complement
    return static_cast<uint8_t>(255 - result);
}

// ====================================== 2D-Parity ======================================

string calculateColumnParity(const vector<string>& data) {
    string colParity;
    for (int i = 0; i < 8; i++) {
        int parity = 0;
        for (int j = 0; j < 8; j++) {
            if (data[j][i] == '1') {
                parity ^= 1;
            }
        }
        colParity += to_string(parity);
    }
    return colParity;
}

// Function to calculate row parity
string calculateRowParity(const vector<string>& data) {
    string rowParity;
    for (int i = 0; i < 9; i++) {
        int parity = 0;
        for (int j = 0; j < 8; j++) {
            if (data[i][j] == '1') {
                parity ^= 1;
            }
        }
        rowParity += to_string(parity);
    }
    return rowParity;
}


// ====================================== Read Data ======================================
vector<string> readData(const string& filename) {
    ifstream inputFile(filename);
    vector<string> data;

    if (!inputFile) {
        cerr << "Failed to open " << filename << endl;
        return data;
    }

    string line;
    while (getline(inputFile, line)) {
        data.push_back(line);
    }

    inputFile.close();
    return data;
}

// ====================================== ERRORS ======================================
string introduceErrors(const string& originalData, const string& errorBits) {
    string modifiedData = originalData;

    for (size_t i = 0; i < modifiedData.length() && i < errorBits.length(); i++) {
        if (errorBits[i] == '1') {
            modifiedData[i] = (modifiedData[i] == '0') ? '1' : '0'; // XOR operation
        }
    }

    return modifiedData;
}

// ====================================== MAIN FUNCTION ======================================

int main() {
    vector<string> data = readData("data.txt");
    for (const string& line : data) {
        // Split the line into binary data and error bits based on space separator
        size_t spacePos = line.find(' ');
        if (spacePos == string::npos) {
            cerr << "Invalid input line: " << line << endl;
            continue;
        }

        string originalData = line.substr(0, spacePos);
        string errorBits = line.substr(spacePos + 1);
                // ==================================== CRC ==========================================
        uint8_t crc8 = calculateCRC8(originalData);
        string originalData8 = originalData;
        originalData8 += bitset<8>(crc8).to_string();

        // Calculate CRC-16 for the original data
        uint16_t crc16 = calculateCRC16(originalData);
        string originalData16 = originalData;
        originalData16 += bitset<16>(crc16).to_string();

        // Add errors
        string receivedData8 = introduceErrors(originalData8, errorBits);
        string receivedData16 = introduceErrors(originalData16, errorBits);

        // Calculate CRC-8 and CRC-16 for the modified data
        uint8_t crc8Received = calculateCRC8(receivedData8);
        uint16_t crc16Received = calculateCRC16(receivedData16);

        // Check if CRCs match to detect errors
        bool crc8Error = (crc8 != crc8Received);
        bool crc16Error = (crc16 != crc16Received);
        
        cout<< "CRC-8 "<< endl;
        if (crc8Error) {
            cout << "CRC: " << bitset<8>(crc8) << "                            " << "Result: Not pass" << endl;
        } else {
            cout << "CRC: " << bitset<8>(crc8) << "                            " << "Result: Pass" << endl;
        }

        cout<< "CRC-16 "<<endl;
        if (crc16Error) {
            cout << "CRC: " << bitset<16>(crc16) << "                    " << "Result: Not pass" << endl;
       } else {
            cout << "CRC: " << bitset<16>(crc16) << "                    " << "Result: Pass" << endl;
        }

        // ==================================== 2D-Parity ==========================================
        size_t spacePos_2dp = line.find(' ');
        if (spacePos_2dp == string::npos) {
            cerr << "Invalid input line: " << line << endl;
            continue;
        }

        string originalData_2dp = line.substr(0, 64);
        string errorBits_2dp = line.substr(65);

        vector<string> data(9);

        // Initialize data blocks
        for (int i = 0; i < 9; i++) {
            data[i] = originalData_2dp.substr(i * 8, 8);
        }

        // Calculate column parity for the 8 data blocks
        string colParity = calculateColumnParity(data);

        // Add column parity as a row and calculate row parity for all 9 blocks
        data.push_back(colParity);
        string rowParity = calculateRowParity(data);

        // Check for errors in row and column parities separately
        bool rowError = (rowParity != errorBits_2dp);
        bool colError = (colParity != errorBits_2dp.substr(0, 8));

        cout << "2D Parity" << endl;
        if (rowError || colError) {
            cout << "Col: " << colParity << ";  Row: " << rowParity << "           " << "Result: Not Pass" << endl;
        } else {
            cout << "Col: " << colParity << ";  Row: " << rowParity << "           " << "Result: Pass" << endl;
        }


        // ==================================== CHECKSUM ==========================================
        size_t spacePos_checksum = line.find(' ');
        if (spacePos_checksum == string::npos) {
            cerr << "Invalid input line: " << line << endl;
            continue;
        }

        string originalData_checksum = line.substr(0, spacePos_checksum);
        string errorBits_checksum = line.substr(spacePos_checksum + 1);

        // Calculate the one-byte version of checksum for the original data
        uint8_t checksum = calculateChecksum(originalData_checksum);

        // cout << originalData << endl << "__________" << endl;

        originalData_checksum += bitset<8>(checksum).to_string();

        // cout << originalData << endl << "__________" << endl;

        // Introduce errors by XORing with error bits
        for (size_t i = 0; i < originalData_checksum.length() && i < errorBits_checksum.length(); i++) {
            if (errorBits_checksum[i] == '1') {
                originalData_checksum[i] = (originalData_checksum[i] == '0') ? '1' : '0'; // XOR operation
            }
        }

        // Calculate the one-byte version of checksum for the modified data
        uint8_t checksumReceived = calculateChecksum(originalData_checksum);

        // Check if checksums match to detect errors
        uint8_t check_rx = 0;
        bool errorDetected_checksum = (check_rx != checksumReceived);

        cout << "Checksum" << endl;
        if (errorDetected_checksum) {
            cout << "Checksum: " << bitset<8>(checksum) << "                       " << "Result: Not pass" << endl;
            cout << "==============================================================" << endl;
        } else {
            cout << "Checksum: " << bitset<8>(checksum) << "                       " << "Result: Pass" << endl;
            cout << "===============================================================" << endl;
        }




    }

    return 0;
}
 