#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <vector>
#include <limits>

std::string hexToBinary(const std::string &hex)
{
    std::string binary;
    for (char character : hex)
    {
        std::string toAdd;
        if (character >= '0' && character <= '9')
        {
            switch(character)
            {
            case('0'): toAdd = "0000"; break;
            case('1'): toAdd = "0001"; break;
            case('2'): toAdd = "0010"; break;
            case('3'): toAdd = "0011"; break;
            case('4'): toAdd = "0100"; break;
            case('5'): toAdd = "0101"; break;
            case('6'): toAdd = "0110"; break;
            case('7'): toAdd = "0111"; break;
            case('8'): toAdd = "1000"; break;
            case('9'): toAdd = "1001"; break;
            }
        }
        else if (character <= 'F' && character >= 'A')
        {
            switch(character)
            {
            case('A'): toAdd = "1010"; break;
            case('B'): toAdd = "1011"; break;
            case('C'): toAdd = "1100"; break;
            case('D'): toAdd = "1101"; break;
            case('E'): toAdd = "1110"; break;
            case('F'): toAdd = "1111"; break;
            }
        }
        else assert(false && "INVALID CHARACTER GIVEN FOR hexToBinary");

        binary += toAdd;
    }

    return binary;
}

struct Packet
{
    int version;
    int typeID;
    long long value; // for non-literals (typeID !=4), value is instead the length representing subpackets
    std::string binary;
    std::vector<Packet> subpackets;

    void print() const
    {
        printf("Packet\nVersion: %d\nTypeID: %d\nValue: %lld\nBinary: %s\n", version, typeID, value, binary.c_str());
        if (subpackets.size() > 0)
        {
            printf("Subpackets:\n");
            for (const Packet &packet : subpackets)
                packet.print();
        }
    }

    int getVersionTotal() const
    {
        if (subpackets.size() > 0)
        {
            long long result = version;
            for (const Packet &subpacket : subpackets)
                result += subpacket.getVersionTotal();
            return result;
        }
        return version;
    }

    long long getValue() const
    {
        switch(typeID)
        {
        case(0): // sum
        {
            long long result = 0;
            for (const Packet &subpacket: subpackets) result += subpacket.getValue();
            return result;
        }
        case(1): // product
        {
            long long result = 1;
            for (const Packet &subpacket: subpackets) result *= subpacket.getValue();
            return result;
        }
        case(2): // minimum
        {
            long long result = std::numeric_limits<long long>::max();
            for (const Packet &subpacket : subpackets)
            {
                long long subValue = subpacket.getValue();
                if (subValue < result) result = subValue;
            }
            return result;
        }
        case(3): // maximum;
        {
            long long result = 0; // I'm pretty sure we dont have negative numbers
            for (const Packet &subpacket : subpackets)
            {
                long long subValue = subpacket.getValue();
                if (subValue > result) result = subValue;
            }
            return result;
        }
        case(4): return value; // literal

        // The following cases will always have exactly two subpackets
        // greater than
        case(5): return (subpackets[0].getValue() > subpackets[1].getValue());
        // less than
        case(6): return (subpackets[0].getValue() < subpackets[1].getValue());
        // equal to
        case(7): return (subpackets[0].getValue() == subpackets[1].getValue());
        }

        assert(false && "Unreachable");
    }
};

Packet parsePacketBinary(const std::string &binary)
{
    int version = std::stoi(binary.substr(0, 3), 0, 2);
    int typeID = std::stoi(binary.substr(3, 3), 0, 2);

    if (typeID == 4) // literal value packet
    {
        std::string binaryValue;
        std::string packets = binary.substr(6);
        size_t index = 0;
        while (index < packets.length())
        {
            char flag = packets[index];
            std::string value = packets.substr(index+1U, 4);
            index += 5;
            binaryValue += value;
            if (flag == '0') break;
        }
        return { version, typeID, std::stoll(binaryValue, 0, 2), binary.substr(0, 6) + binary.substr(6, index) };
    }

    // operator packet
    int lengthID = std::stoi(binary.substr(6, 1), 0, 2);
    switch(lengthID)
    {
    case(0):
    {
        size_t length = static_cast<size_t>(std::stoi(binary.substr(7, 15), 0, 2)); // represnting total length of binary subpackets
        std::string subpacketsString = binary.substr(22, length);
        std::vector<Packet> subpackets;
        size_t index = 0;
        while (index < length)
        {
            Packet subpacket = parsePacketBinary(subpacketsString.substr(index));
            index += subpacket.binary.length();
            subpackets.push_back(subpacket);
        }
        return { version, typeID, static_cast<long long>(length), binary.substr(0, 22 + length), subpackets };
    }
    case(1):
    {
        size_t length = static_cast<size_t>(std::stoi(binary.substr(7, 11), 0, 2)); // represnting the total number of subpackets
        std::string subpacketsString = binary.substr(18);
        std::vector<Packet> subpackets;
        size_t index = 0;
        while (subpackets.size() < length)
        {
            Packet subpacket = parsePacketBinary(subpacketsString.substr(index));
            index += subpacket.binary.length();
            subpackets.push_back(subpacket);
        }
        return { version, typeID, static_cast<long long>(length), binary.substr(0, 18 + index), subpackets };
    }
    }

    assert(false && "UNREACHABLE");
}

Packet parsePacketHex(const std::string &hex) { return parsePacketBinary(hexToBinary(hex)); }

// Hardcoded tests to make sure parser works properly
void test()
{
    std::vector<std::pair<std::string, long long>> tests = {
        {"8A004A801A8002F478", 16},
        {"620080001611562C8802118E34", 12},
        {"C0015000016115A2E0802F182340", 23},
        {"A0016C880162017C3686B18A3D4780", 31}
    };

    int successes = 0;
    for (auto &[hex, result] : tests)
    {
        Packet packet = parsePacketHex(hex);
        long long versionTotal = packet.getVersionTotal();
        std::cout << "Result: " << versionTotal << ", Expected: " << result << '\n';
        std::cout << ( versionTotal == result ? "Succeeded\n" : "Failed\n" );
        successes += (versionTotal == result );
    }
    std::cout << "Successes: " << successes << "/" << tests.size() << '\n';
}
void test2()
{
    std::vector<std::pair<std::string, long long>> tests = {
        {"C200B40A82", 3},
        {"04005AC33890", 54},
        {"880086C3E88112", 7},
        {"CE00C43D881120", 9},
        {"D8005AC2A8F0", 1},
        {"F600BC2D8F", 0},
        {"9C005AC2F8F0", 0},
        {"9C0141080250320F1802104A08", 1}
    };

    int successes = 0;
    for (auto &[hex, result] : tests)
    {
        Packet packet = parsePacketHex(hex);
        long long value = packet.getValue();
        std::cout << hex << '\n';
        std::cout << "Result: " << value << ", Expected: " << result << '\n';
        std::cout << (value == result ? "Succeeded\n" : "Failed\n" );
        successes += (value == result);
    }
    std::cout << "Successes: " << successes << "/" << tests.size() << '\n';
}

void doProblem()
{
    std::string hex;
    std::fstream input("input16");
    getline(input, hex);
    input.close();

    Packet packet = parsePacketHex(hex);
    std::cout << "Result of total versions in hierarchy is " << packet.getVersionTotal() << '\n';
    std::cout << "The value of the packet hierarchy evaluated is " << packet.getValue() << '\n';
}

int main()
{
    // Uncomment to execute tests
    // test();
    // test2();
    doProblem();
    
    return 0;
}