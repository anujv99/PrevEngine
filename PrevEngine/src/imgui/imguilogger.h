#pragma once

#include "imgui.h"


//Copied directly from imgui_demo.cpp and modified for convinience

struct ImGuiAppLog {
private:
	struct MessageLog {
		std::string m_Message;
		ImVec4 m_Color;
	};
public:
	std::vector<MessageLog> Messages;
	ImGuiTextFilter     Filter;
	bool                ScrollToBottom;

	void Clear() {
		Messages.clear();
	}

	void AddLog(ImVec4 color, std::string message) {
		Messages.push_back({message, color});
	}

	void Draw(const char* title, bool* p_open = NULL) {
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open)) {
			ImGui::End();
			return;
		}
		if (ImGui::Button("Clear")) Clear();
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);
		ImGui::NewLine();
		ImGui::Checkbox("Auto Scroll", &ScrollToBottom);
		ImGui::Separator();
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (copy)
			ImGui::LogToClipboard();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		if (Filter.IsActive()) {
			for (auto &message : Messages) {
				if (Filter.PassFilter(message.m_Message.c_str())) {
					ImGui::PushStyleColor(ImGuiCol_Text, message.m_Color);
					ImGui::TextUnformatted(message.m_Message.c_str());
					ImGui::PopStyleColor();
				}
			}
		} else {
			for (auto &message : Messages) {
				ImGui::PushStyleColor(ImGuiCol_Text, message.m_Color);
				ImGui::TextUnformatted(message.m_Message.c_str());
				ImGui::PopStyleColor();
			}
		}

		ImGui::PopStyleVar();

		if (ScrollToBottom) {
			ImGui::SetScrollHereY(1.0f);
		}
		ImGui::EndChild();
		ImGui::End();
	}
};