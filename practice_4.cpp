#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// 将十进制转换为二进制表示
std::string decToBinary(int n) {
    std::string binaryNum;
    while (n > 0) {
        binaryNum.push_back('0' + (n % 2)); // 将当前位存入字符串
        n /= 2;                            // 取下一位
    }
    // 反转以获得正确的二进制表示
    std::reverse(binaryNum.begin(), binaryNum.end());
    return binaryNum;
}

// 生成 k-ary 和窗口表示
std::vector<std::string> k_ary_and_window_representation(int e, int k) {
    std::string binary = decToBinary(e); // 将 e 转换为二进制字符串

    // 根据窗口大小 k 对二进制字符串进行填充
    int pad = k - (binary.length() % k);
    if (pad != k) { // 若需要填充，则前面补零
        binary = std::string(pad, '0') + binary;
    }

    // 将二进制字符串分成长度为 k 的块
    std::vector<std::string> chunks;
    for (int i = 0; i < binary.length(); i += k) {
        chunks.push_back(binary.substr(i, k)); // 每次截取长度为 k 的子串
    }

    return chunks;
}int main() {
    int e, k;

    std::cout << "请输入一个正整数 e：";
    std::cin >> e;

    std::cout << "请输入窗口大小 k：";
    std::cin >> k;

    // 生成 k-ary 和窗口表示
    std::vector<std::string> k_ary_representation = k_ary_and_window_representation(e, k);

    // 输出结果
    std::cout << "整数 " << e << " 的 k-ary 和窗口宽度为 " << k << " 的译码表示为：" << std::endl;
    for (size_t i = 0; i < k_ary_representation.size(); ++i) {
        std::cout << "窗口 " << i + 1 << ": " << k_ary_representation[i] << std::endl;
    }

    return 0;
}