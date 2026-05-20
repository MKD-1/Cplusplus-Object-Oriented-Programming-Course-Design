#pragma once
#include "Auto.h"

namespace MyWindows {
	void RenderUI();
}
class Employee;
class NormalEmployee;
class NormalManager;
class SalesWorker;
class SalesEmployee;
class SalesManager;
class Department;
class NormalDepartment;
class SalesDepartment;
class HRManagementSystem;

// 菜单高度
enum MenuHeight {
	MenuHeight_MainMenu = 5,
	MenuHeight_DepartmentMenu = 4,
	MenuHeight_EmployeeMenu = 4,
	MenuHeight_QueryMenu = 3,
	MenuHeight_StatisticsMenu = 2
};
// 职位
enum Manage_ {
	Manage_NormalEmployee = 1,
	Manage_NormalManager = 2,
	Manage_SalesEmployee = 3,
	Manage_SalesManager = 4,
	Manage_Other = 5
};
// 主菜单选项
enum MainMenuOptions {
	MainMenuOptions_Department = 1,
	MainMenuOptions_Employee = 2,
	MainMenuOptions_Query = 3,
	MainMenuOptions_Statistics = 4,
	MainMenuOptions_EXIT = 5
};
// 部门管理菜单选项
enum DepartmentMenuOptions {
	DepartmentMenuOptions_AddDepartment = 1,
	DepartmentMenuOptions_DeleteDepartment = 2,
	DepartmentMenuOptions_UpdateDepartment = 3,
	DepartmentMenuOptions_Return = 4
};
// 人员管理菜单选项
enum EmployeeMenuOptions {
	EmployeeMenuOptions_AddEmployee = 1,
	EmployeeMenuOptions_DeleteEmployee = 2,
	EmployeeMenuOptions_UpdateEmployee = 3,
	EmployeeMenuOptions_Return = 4
};
// 信息查询菜单选项
enum QueryMenuOptions {
	QueryMenuOptions_QueryEmployee = 1,
	QueryMenuOptions_QueryDepartment = 2,
	QueryMenuOptions_Return = 3
};
// 信息统计菜单选项
enum StatisticsMenuOptions {
	StatisticsMenuOptions_All = 1,
	StatisticsMenuOptions_Return = 2
};
// 员工基类
class Employee {
protected:
	// 所属部门ID
	std::wstring departmentID;
	// 员工ID
	std::wstring employeeID;
	// 员工姓名
	std::wstring name;
	// 职位
	Manage_ position = Manage_Other; 
	// 工作小时数
	int  workHours;
	// 计算工资的纯虚函数
	virtual double  calSalary() = 0;
public:
	Employee(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _workHours) : departmentID(_departmentID), employeeID(_employeeID), name(_name), workHours(_workHours) {}
	Employee(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name) : departmentID(_departmentID), employeeID(_employeeID), name(_name), workHours(0) {}

	std::wstring getDepartmentID() const { return departmentID; }
	std::wstring getEmployeeID() const { return employeeID; }
	std::wstring getName() const { return name; }
	Manage_ getPosition() const { return position; }
	void setPosition(Manage_ pos) { position = pos; }
	int getWorkHours() const { return workHours; }
	static std::wstring getPosition(Manage_ m) {
		if (m == Manage_NormalEmployee) return L"普通员工";
		else if (m == Manage_NormalManager) return L"普通经理";
		else if (m == Manage_SalesEmployee) return L"销售员工";
		else if (m == Manage_SalesManager) return L"销售经理";
		else return L"未知职位";
	}
};
// 普通员工类
class NormalEmployee : public Employee {
public:
	NormalEmployee(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _workHours) :Employee(_departmentID, _employeeID, _name, _workHours) {};
	double  calSalary() override;
};
// 普通经理类
class NormalManager : public Employee {
public:
	double  calSalary() override;
	NormalManager(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _workHours) :Employee(_departmentID, _employeeID, _name, _workHours) {};
};
// 销售类
class SalesWorker {
protected:
	double  monthlySales;
public:
	SalesWorker(int _monthlySales) : monthlySales(_monthlySales) {}
	int getMonthlySales() { return monthlySales; };
};
// 销售员工类
class SalesEmployee : public Employee, public SalesWorker {
public:
	double calSalary() override;
	SalesEmployee(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _monthlySales) :Employee(_departmentID, _employeeID, _name), SalesWorker(_monthlySales) {}
};
// 销售经理类
class SalesManager : public Employee, public SalesWorker {
	// 指向所属销售部门的指针
	SalesDepartment* department;
public:
	double calSalary() override;
	SalesManager(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _monthlySales) :Employee(_departmentID, _employeeID, _name), SalesWorker(_monthlySales), department(nullptr) {}
};

// 部门基类
class Department {
private:
	// 部门ID
	std::wstring	departmentID;
	// 部门名称
	std::wstring departmentName;
	// 部门经理ID
	std::wstring departmentManagerID;
public:
	Department(const std::wstring& id, const std::wstring& name) : departmentID(id), departmentName(name), departmentManagerID(L"") {}
	const std::wstring& getDepartmentID() const;
	const std::wstring& getDepartmentName() const;
	const std::wstring& getDepartmentManagerID() const;
	void setDepartmentID(const std::wstring& id);
	void setDepartmentName(const std::wstring& name);
};
// 普通部门
class NormalDepartment :public Department {
private:
	// 部门总工作小时数
	int departmentTotalWorkHours;
public:
	NormalDepartment(const std::wstring& id, const std::wstring& name);
	// 获取部门总工作小时数
	int getDepartmentTotalWorkHours();
};
// 销售部门
class SalesDepartment :public Department {
private:
	// 销售部门总销售额
	int salesDepartmentTotalSales;
public:
	SalesDepartment(const std::wstring& id, const std::wstring& name);
	// 获取销售部门总销售额
	int	getSalesDepartmentTotalSales();
};

// 人力资源管理系统
class HRManagementSystem {
private:
	// 部门和员工数据
	std::unordered_map<std::wstring, std::unique_ptr<NormalDepartment>> normalDepartments;
	std::unordered_map<std::wstring, std::unique_ptr<SalesDepartment>> salesDepartments;
	std::unordered_map<std::wstring, std::unique_ptr<NormalEmployee>> normalEmployees;
	std::unordered_map<std::wstring, std::unique_ptr<NormalManager>> normalManagers;
	std::unordered_map<std::wstring, std::unique_ptr<SalesEmployee>> salesEmployees;
	std::unordered_map<std::wstring, std::unique_ptr<SalesManager>> salesManagers;
public:
	HRManagementSystem();
	void MainMenu();
	void MainLoop();
	void DepartmentMenu();
	void DepartmentLoop();
	void DepartmentAddMenu();
	void DepartmentAddLoop();
	void DepartmentDeleteLoop();
	void DepartmentUpdateLoop();
	void EmployeeMenu();
	void EmployeeLoop();
	void EmployeeAddLoop();
	void EmployeeDeleteLoop();
	void EmployeeUpdateLoop();
	void QueryMenu();
	void QueryLoop();
	void QueryEmployeeMenu();
	void QueryEmployeeLoop();
	void QueryDepartmentMenu();
	void QueryDepartmentLoop();
	void StatisticsMenu();
	void StatisticsLoop();
	void StatisticsAllMenu();
	void StatisticsAllLoop();
	void Exit();
};
