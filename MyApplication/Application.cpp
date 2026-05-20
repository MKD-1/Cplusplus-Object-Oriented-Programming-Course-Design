#include "Application.h"
#include "Console.h"

namespace MyWindows {
	void RenderUI() {
		static bool windows_open = true;
		static float dragfloat = 0.0f;
		if (windows_open) {
			ImGui::Begin(u8"window 窗口", &windows_open);
			ImGui::DragFloat(u8"拖动浮点数", &dragfloat, 0.01f, 0.0f, 10.0f);
			ImGui::SliderFloat(u8"滑动浮点数", &dragfloat, 0.0f, 20.0f);
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(0, 0), ImVec2(300, 300), IM_COL32(0, 245, 255, 255), 10.0f);

		} else {
			return;
		}

		ImGui::End();
	}
}


NormalDepartment::NormalDepartment(const std::wstring& id, const std::wstring& name) : Department(id, name), departmentTotalWorkHours(0) {}
int NormalDepartment::getDepartmentTotalWorkHours() {
	return departmentTotalWorkHours;
}
const std::wstring& Department::getDepartmentID() const { return departmentID; }
const std::wstring& Department::getDepartmentName() const { return departmentName; }
const std::wstring& Department::getDepartmentManagerID() const { return departmentManagerID; }
void Department::setDepartmentID(const std::wstring& id) { departmentID = id; }
void Department::setDepartmentName(const std::wstring& name) { departmentName = name; }

SalesDepartment::SalesDepartment(const std::wstring& id, const std::wstring& name) : Department(id, name), salesDepartmentTotalSales(0) {}
int SalesDepartment::getSalesDepartmentTotalSales() {
	return salesDepartmentTotalSales;
}

double NormalEmployee::calSalary() {
	return 20 * workHours;
}

double NormalManager::calSalary() {
	return 10000;
}

double SalesEmployee::calSalary() {
	return 0.001 * monthlySales;
}
double SalesManager::calSalary() {
	if (department == nullptr) {
		//TODO: log error here instead of just returning 0
		return 0;
	}
	return 5000 + 0.001 * department->getSalesDepartmentTotalSales();
}
HRManagementSystem::HRManagementSystem() = default;
void HRManagementSystem::MainMenu() {
	MyConsole::updateBorder(0, 0, {
		L"1.部门管理",
		L"2.人员管理",
		L"3.信息查询",
		L"4.信息统计",
		L"5.退出"
		});
}
void HRManagementSystem::MainLoop() {
	MyConsole::LOG_s = L"";
	std::string option;
	while (true) {
		MyConsole::clearScreen();
		MainMenu();
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, MenuHeight_MainMenu + 1);
		}
		std::wstring input;
		MainMenuOptions option;
		try {
			std::wcin >> input;
			option = static_cast<MainMenuOptions>(std::stoi(input));
		} catch (...) {
			MyConsole::LOG_s = L"请输入1-5";
			continue;
		}
		switch (option) {
		case MainMenuOptions_Department:
			DepartmentLoop();
			break;
		case MainMenuOptions_Employee:
			EmployeeLoop();
			break;
		case MainMenuOptions_Query:
			QueryLoop();
			break;
		case MainMenuOptions_Statistics:
			StatisticsLoop();
			break;
		case MainMenuOptions_EXIT:
			Exit();
			break;
		default:
			MyConsole::LOG_s = L"请输入1-5";
			continue;
		}

	}
}
void HRManagementSystem::DepartmentMenu() {
	MyConsole::updateBorder(0, 0, {
		L"1.添加部门",
		L"2.删除部门",
		L"3.更新部门",
		L"4.返回"
		});
}
void HRManagementSystem::DepartmentLoop() {
	MyConsole::LOG_s = L"";
	std::string option;
	while (true) {
		MyConsole::clearScreen();
		DepartmentMenu();
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, MenuHeight_DepartmentMenu + 1);
		}
		std::wstring input;
		MainMenuOptions option;
		try {
			std::wcin >> input;
			MyConsole::ClearInputLine();
			option = static_cast<MainMenuOptions>(std::stoi(input));
		} catch (...) {
			MyConsole::LOG_s = L"请输入1-4";
			continue;
		}
		switch (option) {
		case DepartmentMenuOptions_AddDepartment:
			HRManagementSystem::DepartmentAddLoop();
			break;
		case DepartmentMenuOptions_DeleteDepartment:
			HRManagementSystem::DepartmentDeleteLoop();
			break;
		case DepartmentMenuOptions_UpdateDepartment:
			HRManagementSystem::DepartmentUpdateLoop();
			break;
		case DepartmentMenuOptions_Return:
			MyConsole::LOG_s = L"";
			return;
			break;
		default:
			MyConsole::LOG_s = L"请输入1-4";
			continue;
		}
	}
}
void HRManagementSystem::DepartmentAddMenu() {
	MyConsole::updateBorder(0, 0,
		L"部门ID 部门名称 部门类型(1-普通部门,2-销售部门) [回车结束]"
	);
}
void HRManagementSystem::DepartmentAddLoop() {
	MyConsole::LOG_s = L"";
	while (true) {
		MyConsole::clearScreen();
		DepartmentAddMenu();
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, 2);
		}
		std::wstring input;
		try {
			std::getline(std::wcin, input);
			std::wregex pattern(L"^(\\d+)\\s+(\\S+)\\s+(1|2)$");
			std::wsmatch match;
			if (std::regex_match(input, match, pattern)) {
				std::wstring departmentID = match[1];
				std::wstring departmentName = match[2];
				int type = std::stoi(match[3]);
				if (normalDepartments.find(departmentID) != normalDepartments.end() || salesDepartments.find(departmentID) != salesDepartments.end()) {
					MyConsole::LOG_s = L"部门ID已存在";
					continue;
				}
				if (type == 1) {
					normalDepartments[departmentID] = std::make_unique<NormalDepartment>(departmentID, departmentName);
					MyConsole::LOG_s = L"普通部门添加成功";
				} else {
					salesDepartments[departmentID] = std::make_unique<SalesDepartment>(departmentID, departmentName);
					MyConsole::LOG_s = L"销售部门添加成功";
				}
			} else {
				throw std::invalid_argument("输入格式错误");
			}
		} catch (...) {
			MyConsole::LOG_s = L"格式:(\\d+)\\s+(.+)\\s+(1|2)";
			continue;
		}
		break;
	}

}
void HRManagementSystem::DepartmentDeleteLoop() {
	MyConsole::LOG_s = L"";
	MyConsole::clearScreen();
	if (normalDepartments.empty() && salesDepartments.empty()) {
		MyConsole::LOG_s = L"当前没有部门";
		return;
	}
	std::vector<std::pair<std::wstring, std::wstring>> departments;
	departments.reserve(normalDepartments.size() + salesDepartments.size());
	for (const auto& [id, dep] : normalDepartments) {
		departments.emplace_back(id, dep->getDepartmentName());
	}
	for (const auto& [id, dep] : salesDepartments) {
		departments.emplace_back(id, dep->getDepartmentName());
	}
	std::sort(departments.begin(), departments.end(),
		[](const auto& a, const auto& b) {
			return a.first < b.first;
		});
	std::vector<std::wstring> lines;
	lines.reserve(departments.size());
	for (const auto& [id, name] : departments) {
		lines.push_back(L"ID: " + id + L"  名称: " + name);
	}
	while (true) {
		MyConsole::clearScreen();
		MyConsole::updateBorder(0, 0, lines);
		short preY = static_cast<short>(lines.size() + 2), logY = 0;
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, preY - 1);
			logY = 2;
		}
		MyConsole::gotoXY(0, preY + logY);
		std::wcout << L"请输入要删除的部门ID: ";
		std::wstring departmentID;
		std::getline(std::wcin >> std::ws, departmentID);
		if (departmentID == L"q") {
			MyConsole::LOG_s = L"部门删除已取消";
			break;
		} else if (normalDepartments.erase(departmentID) > 0 || salesDepartments.erase(departmentID) > 0) {
			MyConsole::LOG_s = L"部门删除成功";
			return;
		} else {
			MyConsole::LOG_s = L"未找到该部门，请重新输入";
		}
	}
}
void HRManagementSystem::DepartmentUpdateLoop() {
	MyConsole::LOG_s = L"";
	MyConsole::clearScreen();
	if (normalDepartments.empty() && salesDepartments.empty()) {
		MyConsole::LOG_s = L"当前没有部门，无法修改";
		return;
	}
	std::vector<std::wstring> lines;
	for (const auto& [id, dep] : normalDepartments) {
		lines.push_back(L"ID: " + id + L"  名称: " + dep->getDepartmentName() + L"  类型: 普通部门");
	}
	for (const auto& [id, dep] : salesDepartments) {
		lines.push_back(L"ID: " + id + L"  名称: " + dep->getDepartmentName() + L"  类型: 销售部门");
	}
	while (true) {
		// 选择要修改的部门的ID
		bool done = false;
		MyConsole::clearScreen();
		MyConsole::updateBorder(0, 0, lines);
		short preY = static_cast<short>(lines.size() + 2), logY = 0;
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, preY - 1);
			logY = 2;
		}
		MyConsole::gotoXY(0, preY + logY);
		std::wcout << L"请输入要修改的部门ID: ";
		std::wstring oldID;
		std::getline(std::wcin >> std::ws, oldID);
		if (oldID == L"q") {
			MyConsole::LOG_s = L"部门修改已取消";
			return;
		}
		auto normalIt = normalDepartments.find(oldID);
		auto salesIt = salesDepartments.find(oldID);
		std::wstring oldIt = L"";
		if (normalIt != normalDepartments.end()) {
			oldIt = L"ID:" + normalIt->second->getDepartmentID() + L"名称：" + normalIt->second->getDepartmentName();
		} else if (salesIt != salesDepartments.end()) {
			oldIt = L"ID:" + salesIt->second->getDepartmentID() + L"名称：" + salesIt->second->getDepartmentName();
		} else {
			MyConsole::LOG_s = L"未找到该部门，请重新输入";
			continue;
		}
		MyConsole::LOG_s = L"";
		std::wstring choice = L"";
		while (true) {
			// 选择要修改的内容		
			std::vector<std::wstring> options = {
			oldIt,
				L"1. 修改部门ID",
				L"2. 修改部门名称"
			};
			MyConsole::clearScreen();
			MyConsole::updateBorder(0, 0, options);
			preY = static_cast<short>(options.size() + 2), logY = 0;
			if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
				MyConsole::LOG(0, preY - 1);
				logY = 2;
			}
			MyConsole::gotoXY(0, preY + logY);
			std::wcout << L"请选择要修改的内容: ";
			std::getline(std::wcin >> std::ws, choice);
			if (choice == L"q") {
				MyConsole::LOG_s = L"修改选择已取消";
				break;
			}
			if (choice == L"1") {
				// 修改ID
				MyConsole::LOG_s = L"";
				while (true) {
					options = { oldIt };
					MyConsole::clearScreen();
					MyConsole::updateBorder(0, 0, options);
					preY = static_cast<short>(options.size() + 2), logY = 0;
					if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
						MyConsole::LOG(0, preY - 1);
						logY = 2;
					}
					MyConsole::gotoXY(0, preY + logY);
					std::wcout << L"请输入新的部门ID: ";
					std::wstring newID;
					std::getline(std::wcin >> std::ws, newID);
					if (newID == L"q") {
						choice = L"";
						MyConsole::LOG_s = L"部门ID修改已取消";
						break;
					}
					if (!std::regex_match(newID, std::wregex(L"^\\d+$"))) {
						MyConsole::LOG_s = L"部门ID必须为数字";
						continue;
					}
					if (normalDepartments.count(newID) || salesDepartments.count(newID)) {
						MyConsole::LOG_s = L"该部门ID已存在";
						continue;
					}
					if (normalIt != normalDepartments.end()) {
						auto ptr = std::move(normalIt->second);
						normalDepartments.erase(normalIt);
						ptr->setDepartmentID(newID);
						normalDepartments[newID] = std::move(ptr);
					} else {
						auto ptr = std::move(salesIt->second);
						salesDepartments.erase(salesIt);
						ptr->setDepartmentID(newID);
						salesDepartments[newID] = std::move(ptr);
					}
					MyConsole::LOG_s = L"部门ID修改成功";
					done = true;
					break;
				}
			} else if (choice == L"2") {
				// 修改名称
				MyConsole::LOG_s = L"";
				while (true) {
					options = { oldIt };
					MyConsole::clearScreen();
					MyConsole::updateBorder(0, 0, options);
					preY = static_cast<short>(options.size() + 2), logY = 0;
					if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
						MyConsole::LOG(0, preY - 1);
						logY = 2;
					}
					MyConsole::gotoXY(0, preY + logY);
					std::wcout << L"请输入新的部门名称: ";
					std::wstring newName;
					std::getline(std::wcin >> std::ws, newName);
					if (newName == L"q") {
						choice = L"";
						MyConsole::LOG_s = L"部门名称修改已取消";
						break;
					}
					if (newName.empty()) {
						MyConsole::LOG_s = L"部门名称不能为空";
						continue;
					}
					if (normalIt != normalDepartments.end()) {
						normalIt->second->setDepartmentName(newName);
					} else {
						salesIt->second->setDepartmentName(newName);
					}
					MyConsole::LOG_s = L"部门名称修改成功";
					done = true;
					break;
				}
			} else {
				choice = L"";
				MyConsole::LOG_s = L"请输入1或2";
				continue;
			}
			if (done)break;
		}
		if (done)break;
	}
}
void HRManagementSystem::EmployeeMenu() {
	MyConsole::updateBorder(0, 0, {
		L"1.添加员工",
		L"2.删除员工",
		L"3.更新员工",
		L"4.返回"
		});
}
void HRManagementSystem::EmployeeLoop() {
	MyConsole::LOG_s = L"";
	std::string option;
	while (true) {
		MyConsole::clearScreen();
		EmployeeMenu();
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, MenuHeight_EmployeeMenu + 1);
		}
		std::wstring input;
		EmployeeMenuOptions option;
		try {
			std::wcin >> input;
			MyConsole::ClearInputLine();
			option = static_cast<EmployeeMenuOptions>(std::stoi(input));
		} catch (...) {
			MyConsole::LOG_s = L"请输入1-4";
			continue;
		}
		switch (option) {
		case EmployeeMenuOptions_AddEmployee:
			HRManagementSystem::EmployeeAddLoop();
			break;
		case EmployeeMenuOptions_DeleteEmployee:
			HRManagementSystem::EmployeeDeleteLoop();
			break;
		case EmployeeMenuOptions_UpdateEmployee:
			HRManagementSystem::EmployeeUpdateLoop();
			break;
		case EmployeeMenuOptions_Return:
			MyConsole::LOG_s = L"";
			return;
			break;
		default:
			MyConsole::LOG_s = L"请输入1-4";
			continue;
		}
	}
}
void HRManagementSystem::EmployeeAddLoop() {
	MyConsole::LOG_s = L"";
	while (true) {
		bool done = false;
		// 选择添加员工类型
		std::vector<std::wstring> options = {
			L"1. 普通员工",
			L"2. 普通经理",
			L"3. 销售员工",
			L"4. 销售经理"
		};
		MyConsole::clearScreen();
		MyConsole::updateBorder(0, 0, options);
		short preY = static_cast<short>(options.size() + 2), logY = 0;
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, preY - 1);
			logY = 2;
		}
		MyConsole::gotoXY(0, preY + logY);
		std::wcout << L"请选择要添加的员工类型: ";
		std::wstring input;
		std::getline(std::wcin >> std::ws, input);
		if (input == L"q") {
			MyConsole::LOG_s = L"添加员工已取消";
			break;
		}
		int type = 0;
		try {
			type = std::stoi(input);
		} catch (...) {
			MyConsole::LOG_s = L"请输入1-4";
			continue;
		}
		if (type != Manage_NormalEmployee &&
			type != Manage_NormalManager &&
			type != Manage_SalesEmployee &&
			type != Manage_SalesManager) {
			MyConsole::LOG_s = L"请输入1-4";
			continue;
		}
		Manage_ employeeType = static_cast<Manage_>(type);
		bool isNormal = employeeType == Manage_NormalEmployee || employeeType == Manage_NormalManager;
		bool isSales = employeeType == Manage_SalesEmployee || employeeType == Manage_SalesManager;
		if (isNormal && normalDepartments.empty()) {
			MyConsole::LOG_s = L"当前没有普通部门，无法添加普通员工或普通经理";
			continue;
		}
		if (isSales && salesDepartments.empty()) {
			MyConsole::LOG_s = L"当前没有销售部门，无法添加销售员工或销售经理";
			continue;
		}
		std::vector<std::wstring> lines;
		if (isNormal) {
			for (const auto& [id, dep] : normalDepartments) {
				lines.push_back(L"ID: " + id + L"  名称: " + dep->getDepartmentName() + L"  类型: 普通部门");
			}
		} else {
			for (const auto& [id, dep] : salesDepartments) {
				lines.push_back(L"ID: " + id + L"  名称: " + dep->getDepartmentName() + L"  类型: 销售部门");
			}
		}
		MyConsole::LOG_s = L"";
		while (true) {
			// 选择部门
			MyConsole::clearScreen();
			MyConsole::updateBorder(0, 0, lines);
			preY = static_cast<short>(lines.size() + 2), logY = 0;
			if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
				MyConsole::LOG(0, preY - 1);
				logY = 2;
			}
			MyConsole::gotoXY(0, preY + logY);
			std::wcout << L"请输入所属部门ID: ";
			std::wstring departmentID;
			std::getline(std::wcin >> std::ws, departmentID);
			if (departmentID == L"q") {
				MyConsole::LOG_s = L"添加员工已取消";
				break;
			}
			auto normalDepartmentIt = normalDepartments.find(departmentID);
			auto salesDepartmentIt = salesDepartments.find(departmentID);
			if (isNormal && normalDepartmentIt == normalDepartments.end()) {
				MyConsole::LOG_s = L"未找到该普通部门，请重新输入";
				continue;
			}
			if (isSales && salesDepartmentIt == salesDepartments.end()) {
				MyConsole::LOG_s = L"未找到该销售部门，请重新输入";
				continue;
			}
			MyConsole::LOG_s = L"";
			while (true) {
				// 输入员工数据
				std::vector<std::wstring> employeeInputTip;
				if (employeeType == Manage_NormalEmployee) {
					employeeInputTip = {
						L"员工ID 员工姓名 工作时长"
					};
				} else if (employeeType == Manage_NormalManager) {
					employeeInputTip = {
						L"员工ID 员工姓名 工作时长"
					};
				} else if (employeeType == Manage_SalesEmployee) {
					employeeInputTip = {
						L"员工ID 员工姓名 月销售额"
					};
				} else if (employeeType == Manage_SalesManager) {
					employeeInputTip = {
						L"员工ID 员工姓名 月销售额"
					};
				}
				MyConsole::clearScreen();
				MyConsole::updateBorder(0, 0, employeeInputTip);
				preY = static_cast<short>(employeeInputTip.size() + 2), logY = 0;
				if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
					MyConsole::LOG(0, preY - 1);
					logY = 2;
				}
				MyConsole::gotoXY(0, preY + logY);
				std::wcout << L"请输入员工信息: ";
				std::wstring employeeInput;
				std::getline(std::wcin >> std::ws, employeeInput);
				if (employeeInput == L"q") {
					MyConsole::LOG_s = L"添加员工已取消";
					return;
				}
				try {
					std::wsmatch match;
					if (type == Manage_NormalEmployee) {
						std::wregex pattern(L"^(\\d+)\\s+(\\S+)\\s+(\\d+)$");
						if (!std::regex_match(employeeInput, match, pattern)) {
							throw std::invalid_argument("输入格式错误");
						}
						std::wstring employeeID = match[1];
						std::wstring employeeName = match[2];
						int workHours = std::stoi(match[3]);
						if (normalEmployees.count(employeeID) ||
							normalManagers.count(employeeID) ||
							salesEmployees.count(employeeID) ||
							salesManagers.count(employeeID)) {
							MyConsole::LOG_s = L"员工ID已存在";
							continue;
						}
						normalEmployees[employeeID] = std::make_unique<NormalEmployee>(
							normalDepartmentIt->second->getDepartmentID(),
							employeeID,
							employeeName,
							workHours
						);
						normalEmployees[employeeID]->setPosition(employeeType);
						MyConsole::LOG_s = L"普通员工添加成功";
						done = true;
					} else if (type == Manage_NormalManager) {
						std::wregex pattern(L"^(\\d+)\\s+(\\S+)\\s+(\\d+)$");
						if (!std::regex_match(employeeInput, match, pattern)) {
							throw std::invalid_argument("输入格式错误");
						}
						std::wstring employeeID = match[1];
						std::wstring employeeName = match[2];
						int workHours = std::stoi(match[3]);
						if (normalEmployees.count(employeeID) ||
							normalManagers.count(employeeID) ||
							salesEmployees.count(employeeID) ||
							salesManagers.count(employeeID)) {
							MyConsole::LOG_s = L"员工ID已存在";
							continue;
						}
						normalManagers[employeeID] = std::make_unique<NormalManager>(
							normalDepartmentIt->second->getDepartmentID(),
							employeeID,
							employeeName,
							workHours
						);
						normalManagers[employeeID]->setPosition(employeeType);
						MyConsole::LOG_s = L"普通经理添加成功";
						done = true;
					} else if (type == Manage_SalesEmployee) {
						std::wregex pattern(L"^(\\d+)\\s+(\\S+)\\s+(\\d+)$");
						if (!std::regex_match(employeeInput, match, pattern)) {
							throw std::invalid_argument("输入格式错误");
						}
						std::wstring employeeID = match[1];
						std::wstring employeeName = match[2];
						int monthlySales = std::stoi(match[3]);
						if (normalEmployees.count(employeeID) ||
							normalManagers.count(employeeID) ||
							salesEmployees.count(employeeID) ||
							salesManagers.count(employeeID)) {
							MyConsole::LOG_s = L"员工ID已存在";
							continue;
						}
						salesEmployees[employeeID] = std::make_unique<SalesEmployee>(
							salesDepartmentIt->second->getDepartmentID(),
							employeeID,
							employeeName,
							monthlySales
						);
						salesEmployees[employeeID]->setPosition(employeeType);
						MyConsole::LOG_s = L"销售员工添加成功";
					} else {
						std::wregex pattern(L"^(\\d+)\\s+(\\S+)\\s+(\\d+)$");
						if (!std::regex_match(employeeInput, match, pattern)) {
							throw std::invalid_argument("输入格式错误");
						}
						std::wstring employeeID = match[1];
						std::wstring employeeName = match[2];
						int monthlySales = std::stoi(match[3]);
						if (normalEmployees.count(employeeID) ||
							normalManagers.count(employeeID) ||
							salesEmployees.count(employeeID) ||
							salesManagers.count(employeeID)) {
							MyConsole::LOG_s = L"员工ID已存在";
							continue;
						}
						salesManagers[employeeID] = std::make_unique<SalesManager>(
							salesDepartmentIt->second->getDepartmentID(),
							employeeID,
							employeeName,
							monthlySales
						);
						salesManagers[employeeID]->setPosition(employeeType);
						MyConsole::LOG_s = L"销售经理添加成功";
					}
				} catch (...) {
					if (type == Manage_NormalEmployee) {
						MyConsole::LOG_s = L"格式:(\\d+)\\s+(\\S+)\\s+(\\d+)";
					} else if (type == Manage_NormalManager) {
						MyConsole::LOG_s = L"格式:(\\d+)\\s+(\\S+)\\s+(\\d+)";
					} else if (type == Manage_SalesEmployee) {
						MyConsole::LOG_s = L"格式:(\\d+)\\s+(\\S+)\\s+(\\d+)";
					} else {
						MyConsole::LOG_s = L"格式:(\\d+)\\s+(\\S+)\\s+(\\d+)";
					}
					continue;
				}
				if (done)break;
			}
			if (done)break;
		}
		if (done)break;
	}

}
void HRManagementSystem::EmployeeDeleteLoop() {
	MyConsole::LOG_s = L"";
	MyConsole::clearScreen();
	if (normalEmployees.empty() && normalManagers.empty() && salesEmployees.empty() && salesManagers.empty()) {
		MyConsole::LOG_s = L"当前没有员工";
		return;
	}
	std::vector<std::wstring> lines;
	for (const auto& [id, employee] : normalEmployees) {
		auto depIt = normalDepartments.find(employee->getDepartmentID());
		std::wstring departmentName = depIt == normalDepartments.end() ? L"未知部门" : depIt->second->getDepartmentName();
		lines.push_back(L"ID: " + employee->getEmployeeID() + L"  姓名: " + employee->getName() + L"  职位: " + Employee::getPosition(employee->getPosition()) + L"  部门ID: " + employee->getDepartmentID() + L"  部门名称: " + departmentName + L"  部门类型: 普通部门");
	}
	for (const auto& [id, employee] : normalManagers) {
		auto depIt = normalDepartments.find(employee->getDepartmentID());
		std::wstring departmentName = depIt == normalDepartments.end() ? L"未知部门" : depIt->second->getDepartmentName();
		lines.push_back(L"ID: " + employee->getEmployeeID() + L"  姓名: " + employee->getName() + L"  职位: " + Employee::getPosition(employee->getPosition()) + L"  部门ID: " + employee->getDepartmentID() + L"  部门名称: " + departmentName + L"  部门类型: 普通部门");
	}
	for (const auto& [id, employee] : salesEmployees) {
		auto depIt = salesDepartments.find(employee->getDepartmentID());
		std::wstring departmentName = depIt == salesDepartments.end() ? L"未知部门" : depIt->second->getDepartmentName();
		lines.push_back(L"ID: " + employee->getEmployeeID() + L"  姓名: " + employee->getName() + L"  职位: " + Employee::getPosition(employee->getPosition()) + L"  部门ID: " + employee->getDepartmentID() + L"  部门名称: " + departmentName + L"  部门类型: 销售部门");
	}
	for (const auto& [id, employee] : salesManagers) {
		auto depIt = salesDepartments.find(employee->getDepartmentID());
		std::wstring departmentName = depIt == salesDepartments.end() ? L"未知部门" : depIt->second->getDepartmentName();
		lines.push_back(L"ID: " + employee->getEmployeeID() + L"  姓名: " + employee->getName() + L"  职位: " + Employee::getPosition(employee->getPosition()) + L"  部门ID: " + employee->getDepartmentID() + L"  部门名称: " + departmentName + L"  部门类型: 销售部门");
	}
	std::sort(lines.begin(), lines.end());
	while (true) {
		MyConsole::clearScreen();
		MyConsole::updateBorder(0, 0, lines);
		short preY = static_cast<short>(lines.size() + 2), logY = 0;
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, preY - 1);
			logY = 2;
		}
		MyConsole::gotoXY(0, preY + logY);
		std::wcout << L"请输入要删除的员工ID: ";
		std::wstring employeeID;
		std::getline(std::wcin >> std::ws, employeeID);
		if (employeeID == L"q") {
			MyConsole::LOG_s = L"员工删除已取消";
			break;
		} else if (normalEmployees.erase(employeeID) > 0 || normalManagers.erase(employeeID) > 0 || salesEmployees.erase(employeeID) > 0 || salesManagers.erase(employeeID) > 0) {
			MyConsole::LOG_s = L"员工删除成功";
			return;
		} else {
			MyConsole::LOG_s = L"未找到该员工，请重新输入";
		}
	}
}
void HRManagementSystem::EmployeeUpdateLoop() {
	MyConsole::LOG_s = L"";
	MyConsole::clearScreen();
	if (normalEmployees.empty() && normalManagers.empty() && salesEmployees.empty() && salesManagers.empty()) {
		MyConsole::LOG_s = L"当前没有员工，无法修改";
		return;
	}
	auto employeeIDExists = [&](const std::wstring& employeeID) {
		return normalEmployees.count(employeeID) ||
			normalManagers.count(employeeID) ||
			salesEmployees.count(employeeID) ||
			salesManagers.count(employeeID);
		};
	auto isNormalPosition = [&](Manage_ position) {
		return position == Manage_NormalEmployee || position == Manage_NormalManager;
		};
	auto isSalesPosition = [&](Manage_ position) {
		return position == Manage_SalesEmployee || position == Manage_SalesManager;
		};
	auto getEmployeeInfo = [&](const std::wstring& employeeID, Manage_& position, std::wstring& departmentID, std::wstring& name, int& value) {
		auto normalEmployeeIt = normalEmployees.find(employeeID);
		if (normalEmployeeIt != normalEmployees.end()) {
			position = Manage_NormalEmployee;
			departmentID = normalEmployeeIt->second->getDepartmentID();
			name = normalEmployeeIt->second->getName();
			value = normalEmployeeIt->second->getWorkHours();
			return true;
		}
		auto normalManagerIt = normalManagers.find(employeeID);
		if (normalManagerIt != normalManagers.end()) {
			position = Manage_NormalManager;
			departmentID = normalManagerIt->second->getDepartmentID();
			name = normalManagerIt->second->getName();
			value = normalManagerIt->second->getWorkHours();
			return true;
		}
		auto salesEmployeeIt = salesEmployees.find(employeeID);
		if (salesEmployeeIt != salesEmployees.end()) {
			position = Manage_SalesEmployee;
			departmentID = salesEmployeeIt->second->getDepartmentID();
			name = salesEmployeeIt->second->getName();
			value = salesEmployeeIt->second->getMonthlySales();
			return true;
		}
		auto salesManagerIt = salesManagers.find(employeeID);
		if (salesManagerIt != salesManagers.end()) {
			position = Manage_SalesManager;
			departmentID = salesManagerIt->second->getDepartmentID();
			name = salesManagerIt->second->getName();
			value = salesManagerIt->second->getMonthlySales();
			return true;
		}
		return false;
		};

	auto eraseEmployee = [&](const std::wstring& employeeID) {
		if (normalEmployees.erase(employeeID) > 0)return true;
		if (normalManagers.erase(employeeID) > 0)return true;
		if (salesEmployees.erase(employeeID) > 0)return true;
		if (salesManagers.erase(employeeID) > 0)return true;
		return false;
		};

	auto insertEmployee = [&](Manage_ position, const std::wstring& employeeID, const std::wstring& name, const std::wstring& departmentID, int value) {
		if (position == Manage_NormalEmployee) {
			if (normalDepartments.find(departmentID) == normalDepartments.end())return false;
			normalEmployees[employeeID] = std::make_unique<NormalEmployee>(
				departmentID,
				employeeID,
				name,
				value
			);
			normalEmployees[employeeID]->setPosition(position);
		} else if (position == Manage_NormalManager) {
			if (normalDepartments.find(departmentID) == normalDepartments.end())return false;
			normalManagers[employeeID] = std::make_unique<NormalManager>(
				departmentID,
				employeeID,
				name,
				value
			);
			normalManagers[employeeID]->setPosition(position);
		} else if (position == Manage_SalesEmployee) {
			if (salesDepartments.find(departmentID) == salesDepartments.end())return false;
			salesEmployees[employeeID] = std::make_unique<SalesEmployee>(
				departmentID,
				employeeID,
				name,
				value
			);
			salesEmployees[employeeID]->setPosition(position);
		} else if (position == Manage_SalesManager) {
			if (salesDepartments.find(departmentID) == salesDepartments.end())return false;
			salesManagers[employeeID] = std::make_unique<SalesManager>(
				departmentID,
				employeeID,
				name,
				value
			);
			salesManagers[employeeID]->setPosition(position);
		} else {
			return false;
		}
		return true;
		};

	auto makeEmployeeLine = [&](const std::wstring& employeeID, const std::wstring& name, Manage_ position, const std::wstring& departmentID, int value) {
		std::wstring valueName = isNormalPosition(position) ? L"工时: " : L"销售额: ";
		return L"ID: " + employeeID +
			L"  姓名: " + name +
			L"  职位: " + Employee::getPosition(position) +
			L"  部门ID: " + departmentID +
			L"  " + valueName + std::to_wstring(value);
		};

	std::vector<std::wstring> lines;
	for (const auto& [id, employee] : normalEmployees) {
		lines.push_back(makeEmployeeLine(
			employee->getEmployeeID(),
			employee->getName(),
			Manage_NormalEmployee,
			employee->getDepartmentID(),
			employee->getWorkHours()
		));
	}
	for (const auto& [id, employee] : normalManagers) {
		lines.push_back(makeEmployeeLine(
			employee->getEmployeeID(),
			employee->getName(),
			Manage_NormalManager,
			employee->getDepartmentID(),
			employee->getWorkHours()
		));
	}
	for (const auto& [id, employee] : salesEmployees) {
		lines.push_back(makeEmployeeLine(
			employee->getEmployeeID(),
			employee->getName(),
			Manage_SalesEmployee,
			employee->getDepartmentID(),
			employee->getMonthlySales()
		));
	}
	for (const auto& [id, employee] : salesManagers) {
		lines.push_back(makeEmployeeLine(
			employee->getEmployeeID(),
			employee->getName(),
			Manage_SalesManager,
			employee->getDepartmentID(),
			employee->getMonthlySales()
		));
	}

	std::sort(lines.begin(), lines.end());

	while (true) {
		bool done = false;
		MyConsole::clearScreen();
		MyConsole::updateBorder(0, 0, lines);
		short preY = static_cast<short>(lines.size() + 2), logY = 0;
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, preY - 1);
			logY = 2;
		}
		MyConsole::gotoXY(0, preY + logY);
		std::wcout << L"请输入要修改的员工ID: ";
		std::wstring oldID;
		std::getline(std::wcin >> std::ws, oldID);
		if (oldID == L"q") {
			MyConsole::LOG_s = L"员工修改已取消";
			return;
		}
		Manage_ position;
		std::wstring departmentID;
		std::wstring name;
		int value = 0;
		if (!getEmployeeInfo(oldID, position, departmentID, name, value)) {
			MyConsole::LOG_s = L"未找到该员工，请重新输入";
			continue;
		}
		std::wstring oldIt = makeEmployeeLine(oldID, name, position, departmentID, value);
		MyConsole::LOG_s = L"";

		while (true) {
			std::vector<std::wstring> options = {
				oldIt,
				L"1. 修改员工ID",
				L"2. 修改员工姓名",
				L"3. 修改职位",
				L"4. 修改所属部门",
				L"5. 修改工时/销售额"
			};
			MyConsole::clearScreen();
			MyConsole::updateBorder(0, 0, options);
			preY = static_cast<short>(options.size() + 2), logY = 0;
			if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
				MyConsole::LOG(0, preY - 1);
				logY = 2;
			}
			MyConsole::gotoXY(0, preY + logY);
			std::wcout << L"请选择要修改的内容: ";
			std::wstring choice;
			std::getline(std::wcin >> std::ws, choice);
			if (choice == L"q") {
				MyConsole::LOG_s = L"修改选择已取消";
				break;
			}
			if (choice == L"1") {
				MyConsole::LOG_s = L"";
				while (true) {
					MyConsole::clearScreen();
					MyConsole::updateBorder(0, 0, { oldIt });
					preY = 3, logY = 0;
					if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
						MyConsole::LOG(0, preY - 1);
						logY = 2;
					}
					MyConsole::gotoXY(0, preY + logY);
					std::wcout << L"请输入新的员工ID: ";
					std::wstring newID;
					std::getline(std::wcin >> std::ws, newID);
					if (newID == L"q") {
						MyConsole::LOG_s = L"员工ID修改已取消";
						break;
					}
					if (!std::regex_match(newID, std::wregex(L"^\\d+$"))) {
						MyConsole::LOG_s = L"员工ID必须为数字";
						continue;
					}
					if (employeeIDExists(newID)) {
						MyConsole::LOG_s = L"员工ID已存在";
						continue;
					}
					eraseEmployee(oldID);
					insertEmployee(position, newID, name, departmentID, value);
					MyConsole::LOG_s = L"员工ID修改成功";
					done = true;
					break;
				}
			} else if (choice == L"2") {
				MyConsole::LOG_s = L"";
				while (true) {
					MyConsole::clearScreen();
					MyConsole::updateBorder(0, 0, { oldIt });
					preY = 3, logY = 0;
					if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
						MyConsole::LOG(0, preY - 1);
						logY = 2;
					}
					MyConsole::gotoXY(0, preY + logY);
					std::wcout << L"请输入新的员工姓名: ";
					std::wstring newName;
					std::getline(std::wcin >> std::ws, newName);
					if (newName == L"q") {
						MyConsole::LOG_s = L"员工姓名修改已取消";
						break;
					}
					if (newName.empty()) {
						MyConsole::LOG_s = L"员工姓名不能为空";
						continue;
					}
					eraseEmployee(oldID);
					insertEmployee(position, oldID, newName, departmentID, value);
					MyConsole::LOG_s = L"员工姓名修改成功";
					done = true;
					break;
				}
			} else if (choice == L"3") {
				MyConsole::LOG_s = L"";
				while (true) {
					std::vector<std::wstring> positionOptions;
					if (isNormalPosition(position)) {
						positionOptions = {
							oldIt,
							L"1. 普通员工",
							L"2. 普通经理"
						};
					} else {
						positionOptions = {
							oldIt,
							L"1. 销售员工",
							L"2. 销售经理"
						};
					}
					MyConsole::clearScreen();
					MyConsole::updateBorder(0, 0, positionOptions);
					preY = static_cast<short>(positionOptions.size() + 2), logY = 0;
					if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
						MyConsole::LOG(0, preY - 1);
						logY = 2;
					}
					MyConsole::gotoXY(0, preY + logY);
					std::wcout << L"请选择新的职位: ";
					std::wstring newPositionInput;
					std::getline(std::wcin >> std::ws, newPositionInput);
					if (newPositionInput == L"q") {
						MyConsole::LOG_s = L"职位修改已取消";
						break;
					}
					int newPositionNumber = 0;
					try {
						newPositionNumber = std::stoi(newPositionInput);
					} catch (...) {
						MyConsole::LOG_s = L"请输入1或2";
						continue;
					}
					Manage_ newPosition = static_cast<Manage_>(newPositionNumber + (isNormalPosition(position) ? 0 : 2));
					eraseEmployee(oldID);
					insertEmployee(newPosition, oldID, name, departmentID, value);
					MyConsole::LOG_s = L"员工职位修改成功";
					done = true;
					break;
				}
			} else if (choice == L"4") {
				MyConsole::LOG_s = L"";
				std::vector<std::wstring> departmentLines;
				if (isNormalPosition(position)) {
					if (normalDepartments.empty()) {
						MyConsole::LOG_s = L"当前没有普通部门";
						continue;
					}
					for (const auto& [id, dep] : normalDepartments) {
						departmentLines.push_back(L"ID: " + id + L"  名称: " + dep->getDepartmentName() + L"  类型: 普通部门");
					}
				} else {
					if (salesDepartments.empty()) {
						MyConsole::LOG_s = L"当前没有销售部门";
						continue;
					}
					for (const auto& [id, dep] : salesDepartments) {
						departmentLines.push_back(L"ID: " + id + L"  名称: " + dep->getDepartmentName() + L"  类型: 销售部门");
					}
				}
				while (true) {
					MyConsole::clearScreen();
					MyConsole::updateBorder(0, 0, departmentLines);
					preY = static_cast<short>(departmentLines.size() + 2), logY = 0;
					if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
						MyConsole::LOG(0, preY - 1);
						logY = 2;
					}
					MyConsole::gotoXY(0, preY + logY);
					std::wcout << L"请输入新的所属部门ID: ";
					std::wstring newDepartmentID;
					std::getline(std::wcin >> std::ws, newDepartmentID);
					if (newDepartmentID == L"q") {
						MyConsole::LOG_s = L"所属部门修改已取消";
						break;
					}
					if (isNormalPosition(position) && normalDepartments.find(newDepartmentID) == normalDepartments.end()) {
						MyConsole::LOG_s = L"未找到该普通部门，请重新输入";
						continue;
					}
					if (isSalesPosition(position) && salesDepartments.find(newDepartmentID) == salesDepartments.end()) {
						MyConsole::LOG_s = L"未找到该销售部门，请重新输入";
						continue;
					}
					eraseEmployee(oldID);
					insertEmployee(position, oldID, name, newDepartmentID, value);
					MyConsole::LOG_s = L"所属部门修改成功";
					done = true;
					break;
				}
			} else if (choice == L"5") {
				MyConsole::LOG_s = L"";
				while (true) {
					MyConsole::clearScreen();
					MyConsole::updateBorder(0, 0, { oldIt });
					preY = 3, logY = 0;
					if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
						MyConsole::LOG(0, preY - 1);
						logY = 2;
					}
					MyConsole::gotoXY(0, preY + logY);
					if (isNormalPosition(position)) {
						std::wcout << L"请输入新的工作时长: ";
					} else {
						std::wcout << L"请输入新的月销售额: ";
					}
					std::wstring newValueInput;
					std::getline(std::wcin >> std::ws, newValueInput);
					if (newValueInput == L"q") {
						MyConsole::LOG_s = isNormalPosition(position) ? L"工时修改已取消" : L"销售额修改已取消";
						break;
					}
					if (!std::regex_match(newValueInput, std::wregex(L"^\\d+$"))) {
						MyConsole::LOG_s = isNormalPosition(position) ? L"工作时长必须为数字" : L"月销售额必须为数字";
						continue;
					}
					int newValue = std::stoi(newValueInput);
					eraseEmployee(oldID);
					insertEmployee(position, oldID, name, departmentID, newValue);
					MyConsole::LOG_s = isNormalPosition(position) ? L"工作时长修改成功" : L"月销售额修改成功";
					done = true;
					break;
				}
			} else {
				MyConsole::LOG_s = L"请输入1-5";
				continue;
			}
			if (done)break;
		}
		if (done)break;
	}

}

void HRManagementSystem::QueryMenu() {
	MyConsole::updateBorder(0, 0, {
		L"1.查询员工",
		L"2.查询部门",
		L"3.返回"
		});
}
void HRManagementSystem::QueryLoop() {
	MyConsole::LOG_s = L"";
	std::string option;
	while (true) {
		MyConsole::clearScreen();
		QueryMenu();
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, MenuHeight_QueryMenu + 1);
		}
		std::wstring input;
		QueryMenuOptions option;
		try {
			std::wcin >> input;
			option = static_cast<QueryMenuOptions>(std::stoi(input));
		} catch (...) {
			MyConsole::LOG_s = L"请输入1-3";
			continue;
		}
		switch (option) {
		case QueryMenuOptions_QueryEmployee:
			HRManagementSystem::QueryEmployeeLoop();
			break;
		case QueryMenuOptions_QueryDepartment:
			HRManagementSystem::QueryDepartmentLoop();
			break;
		case QueryMenuOptions_Return:
			MyConsole::LOG_s = L"";
			return;
			break;
		default:
			MyConsole::LOG_s = L"请输入1-3";
			continue;
		}
	}
}
void HRManagementSystem::QueryEmployeeLoop() {
}
void HRManagementSystem::QueryDepartmentLoop() {
}
void HRManagementSystem::StatisticsMenu() {
	MyConsole::updateBorder(0, 0, {
		{L"1.统计所有"},
		{L"2.返回"}
		});
}
void HRManagementSystem::StatisticsLoop() {
	MyConsole::LOG_s = L"";
	std::string option;
	while (true) {
		MyConsole::clearScreen();
		StatisticsMenu();
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, MenuHeight_StatisticsMenu + 1);
		}
		std::wstring input;
		StatisticsMenuOptions option;
		try {
			std::wcin >> input;
			option = static_cast<StatisticsMenuOptions>(std::stoi(input));
		} catch (...) {
			MyConsole::LOG_s = L"请输入1-2";
			continue;
		}
		switch (option) {
		case StatisticsMenuOptions_All:
			HRManagementSystem::StatisticsAllLoop();
			break;
		case StatisticsMenuOptions_Return:
			MyConsole::LOG_s = L"";
			return;
			break;
		default:
			MyConsole::LOG_s = L"请输入1-2";
			continue;
		}
	}

}
void HRManagementSystem::StatisticsAllLoop() {

}
void HRManagementSystem::Exit() {
	MyConsole::clearScreen();
	MyConsole::MyPrint(L"谢谢使用，再见！");
	exit(0);
}