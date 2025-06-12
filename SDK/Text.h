#pragma once
#include <string>
#include <vector>
namespace Text {
	static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	/*
	* 文本_分割
	* @param text 待分割的文本
	* @param delimiters 分隔符
	* @return 分割后的文本数组
	*/
	template<typename... Delimiters>
	std::vector<std::string> text_split(std::string text, Delimiters... delimiters) {
		std::vector<std::string> result;

		// 将所有分隔符合并为一个字符串
		std::string allDelimiters = (std::string(delimiters) + ...);

		// 使用分隔符分割文本
		std::string::size_type pos1 = 0, pos2 = 0;
		while ((pos2 = text.find_first_of(allDelimiters, pos1)) != std::string::npos) {
			if (pos2 > pos1) {
				result.push_back(text.substr(pos1, pos2 - pos1));
			}
			pos1 = pos2 + 1; // 跳过分隔符
		}
		if (pos1 < text.length()) {
			result.push_back(text.substr(pos1));
		}

		return result;
	}
	/*
	* 文本_合并
	* @param text_list 待合并的文本数组
	* @param delimiter 连接符
	* @return 合并后的文本
	*/
	std::string __stdcall text_join(std::vector<std::string> text_list, std::string delimiter = "");
	/*
	* 文本_长度分割
	* @param text 待分割的文本
	* @param length 分割长度
	* @return 分割后的文本数组
	*/
	std::vector<std::string> __stdcall text_split_len(std::string text, size_t length = 1);
	/*
	* 文本_左侧删除
	* @param text 待删除的文本
	* @param length 待删除的长度
	* @return 删除后的文本
	*/
	std::string __stdcall text_left_del(std::string text, size_t length = 1);
	/*
	* 文本_右侧删除
	* @param text 待删除的文本
	* @param length 待删除的长度
	* @return 删除后的文本
	*/
	std::string __stdcall text_right_del(std::string text, size_t length = 1);
	/*
	* 文本_寻找指定文本
	* @param text 被查找的文本
	* @param find_text 待查找的文本
	* @return 找到的位置
	*/
	int __stdcall text_find(std::string text, std::string find_text);
	/*
	* 文本_替换
	* @param text 待替换的文本
	* @param find_text 待查找的文本
	* @param replace_text 用作替换的文本
	* @param count 最大替换次数，-1表示全部替换
	* @return 替换后的文本
	*/
	std::string __stdcall text_replace(std::string text, std::string find_text, std::string replace_text, size_t count = -1);
	/*
	* 文本_转宽字符(不支持中文)
	* @param text 待转换的文本
	* @return 转换后的文本
	*/
	std::wstring __stdcall text_to_wstr(std::string text);
	/*
	* 文本_转窄字符
	* @param text 待转换的文本
	* @return 转换后的文本
	*/
	std::string __stdcall text_to_str(std::wstring text);
	/*
	* 文本_取随机字符串(数字)
	* @param length 字符串长度 (默认10)
	* @return 随机字符串
	*/
	std::string __stdcall text_random_num(size_t length = 10);
	/*
	* 文本_取随机字符串(字母)
	* @param length 字符串长度 (默认10)
	* @return 随机字符串
	*/
	std::string __stdcall text_random_alpha(size_t length = 10);
	/*
	* 文本_取随机字符串(字母+数字)
	* @param length 字符串长度 (默认10)
	* @return 随机字符串
	*/
	std::string __stdcall text_random_alnum(size_t length = 10);
	/*
	* 文本_取随机字符串(字母+数字+特殊字符)
	* @param length 字符串长度 (默认10)
	* @return 随机字符串
	*/
	std::string __stdcall text_random_special(size_t length = 10);
	//====================================编码转换====================================
	/*
	* 文本_base64编码
	* @param text 待编码的文本
	* @return 编码后的文本
	*/
	std::string __stdcall text_base64_encode(std::string text);
	/*
	* 文本_base64解码
	* @param text 待解码的文本
	* @return 解码后的文本
	*/
	std::string __stdcall text_base64_decode(std::string text);
	/*
	* 文本_GB2312编码转UTF8
	* @param text 待转换的文本
	* @return 转换后的文本
	*/
	std::string __stdcall text_gb2312_to_utf8(std::string text);
	/*
	* 文本_UTF8编码转GB2312
	* @param text 待转换的文本
	* @return 转换后的文本
	*/
	std::string __stdcall text_utf8_to_gb2312(std::string text);
	/*
	* 文本_Unicode编码转UTF8
	* @param text 待转换的文本
	* @return 转换后的文本
	*/
	std::string __stdcall text_unicode_to_utf8(std::wstring text);
	/*
	* 文本_UTF8编码转Unicode
	* @param text 待转换的文本
	* @return 转换后的文本
	*/
	std::wstring __stdcall text_utf8_to_unicode(std::string text);
	/*
	* 文本_Unicode编码转ASCII
	* @param text 待转换的文本
	* @return 转换后的文本
	*/
	std::string __stdcall text_unicode_to_ascii(std::wstring text);
	/*
	* 文本_ASCII编码转Unicode
	* @param text 待转换的文本
	* @return 转换后的文本
	*/
	std::wstring __stdcall text_ascii_to_unicode(std::string text);
	//====================================进制转换====================================
	/*
	* 文本_10进制转16进制
	* @param num 待转换的十进制数
	* @return 转换后的任意进制数
	*/
	std::string __stdcall text_10_to_16(__int64 num);
	/*
	* 文本_16进制转10进制
	* @param hex 待转换的十六进制数
	* @return 转换后的十进制数
	*/
	__int64 __stdcall text_16_to_10(std::string hex);
	/*
	* 文本_10进制转2进制
	* @param num 待转换的十进制数
	* @return 转换后的任意进制数
	*/
	std::string __stdcall text_10_to_2(size_t num);
	/*
	* 文本_2进制转10进制
	* @param bin 待转换的二进制数
	* @return 转换后的十进制数
	*/
	size_t __stdcall text_2_to_10(std::string bin);
};