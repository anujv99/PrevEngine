#include "pch.h"
#include "imguilayer.h"
#include "essentials/timer.h"
#include "essentials/input.h"
#include "application.h"

#include "imguilogger.h"

namespace prev {

	static bool is_demo				= true;
	static bool is_logging			= true;
	static bool is_miscPropsWindow	= true;
	static bool changeLogPorps		= false;
	static ImGuiAppLog log;

	ImGuiLayer::ImGuiLayer() {
		m_API = ImGuiWrapper::Initialize();
		m_API->Init();

		{
			m_LogColors[LogLevel::PV_INFO]		= ImVec4(0, 1, 0, 1);
			m_LogColors[LogLevel::PV_TRACE]		= ImVec4(0, 1, 1, 1);
			m_LogColors[LogLevel::PV_WARN]		= ImVec4(1, 1, 0, 1);
			m_LogColors[LogLevel::PV_ERROR]		= ImVec4(1, 0, 0, 1);
			m_LogColors[LogLevel::PV_CRITICAL]	= ImVec4(1, 0, 1, 1);
		}

		Log::SetLoggerCallbackFunction([this](std::string s, LogLevel l)->void {
			log.AddLog(l, s);
		});

		m_B2DebugLayer = Application::GetApplicationInstance()->GetB2Draw();

	}

	ImGuiLayer::~ImGuiLayer() {
		delete m_API;
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach() {
		PV_CORE_INFO("ImGui Layer attached!");
	}

	void ImGuiLayer::OnDetach() {
		PV_CORE_INFO("ImGui Layer detached!");
	}

	void ImGuiLayer::OnUpdate() {
		
		m_API->NewFrame();
		
		if (is_demo)
			ImGui::ShowDemoWindow(&is_demo);

		if (is_logging)
			ShowLogger();

		if (is_miscPropsWindow)
			ShowMiscPropsWindow();

		//ShowMainMenuBar();

		m_API->Render();
	}

	void ImGuiLayer::OnEvent(Event &event) {
		m_API->OnEvent(event);
	}

	void ImGuiLayer::ShowMainMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Misc")) {
				ImGui::MenuItem("Show Log", NULL, &is_logging);
				ImGui::MenuItem("Show Demo Window", NULL, &is_demo);
				ImGui::MenuItem("Change Properties", NULL, &is_miscPropsWindow);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Log")) {
				ImGui::MenuItem("Show Log", NULL, &is_logging);
				ImGui::MenuItem("Change Log Properties", NULL, &changeLogPorps);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void ImGuiLayer::ShowMiscPropsWindow() {
		ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Misc Properties", &is_miscPropsWindow)) {

			if (ImGui::CollapsingHeader("Box2D Debug Draw Props")) {
				if (m_B2DebugLayer != nullptr) {
					static bool b2_debug_draw_enabled = true;
					static unsigned int current_flags = 0;
					if (ImGui::Checkbox("Box2D Debug Enabled", &b2_debug_draw_enabled)) {
						unsigned int flags = m_B2DebugLayer->GetFlags();
						m_B2DebugLayer->SetFlags(current_flags);
						current_flags = flags;
					}
					if (b2_debug_draw_enabled) {
						static bool is_shape_bit	= true;
						static bool is_joint_bit	= true;
						static bool is_aabb_bit		= true;
						static bool is_pair_bit		= true;
						static bool is_cm_bit		= true; // Center of mass bit
						if (ImGui::Checkbox("Shape Bit", &is_shape_bit)) {
							if (is_shape_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_shapeBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_shapeBit);
							}
						} else if (ImGui::Checkbox("Joint Bit", &is_joint_bit)) {
							if (is_joint_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_jointBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_jointBit);
							}
						} else if (ImGui::Checkbox("AABB Bit", &is_aabb_bit)) {
							if (is_aabb_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_aabbBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_aabbBit);
							}
						} else if (ImGui::Checkbox("Pait Bit", &is_pair_bit)) {
							if (is_pair_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_pairBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_pairBit);
							}
						} else if (ImGui::Checkbox("Center of Mass Bit", &is_cm_bit)) {
							if (is_cm_bit) {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() + b2Draw::e_centerOfMassBit);
							} else {
								m_B2DebugLayer->SetFlags(m_B2DebugLayer->GetFlags() - b2Draw::e_centerOfMassBit);
							}
						}
					}
				}
			}

		}
		ImGui::End();
	}

	void ImGuiLayer::ShowLogger() {
		log.Draw("Log", m_LogColors, &is_logging);
		if (changeLogPorps) {
			if (ImGui::Begin("Log Props", &changeLogPorps)) {
				if (ImGui::CollapsingHeader("Log Level Colors")) {
					ImGui::ColorEdit4("Info", (float*)&m_LogColors[LogLevel::PV_INFO]);
					ImGui::ColorEdit4("Trace", (float*)&m_LogColors[LogLevel::PV_TRACE]);
					ImGui::ColorEdit4("Warn", (float*)&m_LogColors[LogLevel::PV_WARN]);
					ImGui::ColorEdit4("Error", (float*)&m_LogColors[LogLevel::PV_ERROR]);
					ImGui::ColorEdit4("Critical", (float*)&m_LogColors[LogLevel::PV_CRITICAL]);
				}
				ImGui::End();
			}
		}
	}
	
}