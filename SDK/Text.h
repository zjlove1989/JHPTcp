#pragma once
#include <string>
#include <vector>
namespace Text {
	static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	/*
	* �ı�_�ָ�
	* @param text ���ָ���ı�
	* @param delimiters �ָ���
	* @return �ָ����ı�����
	*/
	template<typename... Delimiters>
	std::vector<std::string> text_split(std::string text, Delimiters... delimiters) {
		std::vector<std::string> result;

		// �����зָ����ϲ�Ϊһ���ַ���
		std::string allDelimiters = (std::string(delimiters) + ...);

		// ʹ�÷ָ����ָ��ı�
		std::string::size_type pos1 = 0, pos2 = 0;
		while ((pos2 = text.find_first_of(allDelimiters, pos1)) != std::string::npos) {
			if (pos2 > pos1) {
				result.push_back(text.substr(pos1, pos2 - pos1));
			}
			pos1 = pos2 + 1; // �����ָ���
		}
		if (pos1 < text.length()) {
			result.push_back(text.substr(pos1));
		}

		return result;
	}
	/*
	* �ı�_�ϲ�
	* @param text_list ���ϲ����ı�����
	* @param delimiter ���ӷ�
	* @return �ϲ�����ı�
	*/
	std::string __stdcall text_join(std::vector<std::string> text_list, std::string delimiter = "");
	/*
	* �ı�_���ȷָ�
	* @param text ���ָ���ı�
	* @param length �ָ��
	* @return �ָ����ı�����
	*/
	std::vector<std::string> __stdcall text_split_len(std::string text, size_t length = 1);
	/*
	* �ı�_���ɾ��
	* @param text ��ɾ�����ı�
	* @param length ��ɾ���ĳ���
	* @return ɾ������ı�
	*/
	std::string __stdcall text_left_del(std::string text, size_t length = 1);
	/*
	* �ı�_�Ҳ�ɾ��
	* @param text ��ɾ�����ı�
	* @param length ��ɾ���ĳ���
	* @return ɾ������ı�
	*/
	std::string __stdcall text_right_del(std::string text, size_t length = 1);
	/*
	* �ı�_Ѱ��ָ���ı�
	* @param text �����ҵ��ı�
	* @param find_text �����ҵ��ı�
	* @return �ҵ���λ��
	*/
	int __stdcall text_find(std::string text, std::string find_text);
	/*
	* �ı�_�滻
	* @param text ���滻���ı�
	* @param find_text �����ҵ��ı�
	* @param replace_text �����滻���ı�
	* @param count ����滻������-1��ʾȫ���滻
	* @return �滻����ı�
	*/
	std::string __stdcall text_replace(std::string text, std::string find_text, std::string replace_text, size_t count = -1);
	/*
	* �ı�_ת���ַ�(��֧������)
	* @param text ��ת�����ı�
	* @return ת������ı�
	*/
	std::wstring __stdcall text_to_wstr(std::string text);
	/*
	* �ı�_תխ�ַ�
	* @param text ��ת�����ı�
	* @return ת������ı�
	*/
	std::string __stdcall text_to_str(std::wstring text);
	/*
	* �ı�_ȡ����ַ���(����)
	* @param length �ַ������� (Ĭ��10)
	* @return ����ַ���
	*/
	std::string __stdcall text_random_num(size_t length = 10);
	/*
	* �ı�_ȡ����ַ���(��ĸ)
	* @param length �ַ������� (Ĭ��10)
	* @return ����ַ���
	*/
	std::string __stdcall text_random_alpha(size_t length = 10);
	/*
	* �ı�_ȡ����ַ���(��ĸ+����)
	* @param length �ַ������� (Ĭ��10)
	* @return ����ַ���
	*/
	std::string __stdcall text_random_alnum(size_t length = 10);
	/*
	* �ı�_ȡ����ַ���(��ĸ+����+�����ַ�)
	* @param length �ַ������� (Ĭ��10)
	* @return ����ַ���
	*/
	std::string __stdcall text_random_special(size_t length = 10);
	//====================================����ת��====================================
	/*
	* �ı�_base64����
	* @param text ��������ı�
	* @return �������ı�
	*/
	std::string __stdcall text_base64_encode(std::string text);
	/*
	* �ı�_base64����
	* @param text ��������ı�
	* @return �������ı�
	*/
	std::string __stdcall text_base64_decode(std::string text);
	/*
	* �ı�_GB2312����תUTF8
	* @param text ��ת�����ı�
	* @return ת������ı�
	*/
	std::string __stdcall text_gb2312_to_utf8(std::string text);
	/*
	* �ı�_UTF8����תGB2312
	* @param text ��ת�����ı�
	* @return ת������ı�
	*/
	std::string __stdcall text_utf8_to_gb2312(std::string text);
	/*
	* �ı�_Unicode����תUTF8
	* @param text ��ת�����ı�
	* @return ת������ı�
	*/
	std::string __stdcall text_unicode_to_utf8(std::wstring text);
	/*
	* �ı�_UTF8����תUnicode
	* @param text ��ת�����ı�
	* @return ת������ı�
	*/
	std::wstring __stdcall text_utf8_to_unicode(std::string text);
	/*
	* �ı�_Unicode����תASCII
	* @param text ��ת�����ı�
	* @return ת������ı�
	*/
	std::string __stdcall text_unicode_to_ascii(std::wstring text);
	/*
	* �ı�_ASCII����תUnicode
	* @param text ��ת�����ı�
	* @return ת������ı�
	*/
	std::wstring __stdcall text_ascii_to_unicode(std::string text);
	//====================================����ת��====================================
	/*
	* �ı�_10����ת16����
	* @param num ��ת����ʮ������
	* @return ת��������������
	*/
	std::string __stdcall text_10_to_16(__int64 num);
	/*
	* �ı�_16����ת10����
	* @param hex ��ת����ʮ��������
	* @return ת�����ʮ������
	*/
	__int64 __stdcall text_16_to_10(std::string hex);
	/*
	* �ı�_10����ת2����
	* @param num ��ת����ʮ������
	* @return ת��������������
	*/
	std::string __stdcall text_10_to_2(size_t num);
	/*
	* �ı�_2����ת10����
	* @param bin ��ת���Ķ�������
	* @return ת�����ʮ������
	*/
	size_t __stdcall text_2_to_10(std::string bin);
};