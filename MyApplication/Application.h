#pragma once
#include "Auto.h"


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

// 职位
enum Manage_ {
	Manage_NormalEmployee = 1,
	Manage_NormalManager = 2,
	Manage_SalesEmployee = 3,
	Manage_SalesManager = 4,
	Manage_Other = 5
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
	Employee(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _workHours);
	Employee(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name);
	std::wstring getDepartmentID() const;
	std::wstring getEmployeeID() const;
	std::wstring getName() const;
	Manage_ getPosition() const;
	void setPosition(Manage_ pos);
	int getWorkHours() const;
	static std::wstring getPosition(Manage_ m);
};
// 普通员工类
class NormalEmployee : public Employee {
public:
	NormalEmployee(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _workHours);
	double  calSalary() override;
};
// 普通经理类
class NormalManager : public Employee {
public:
	NormalManager(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _workHours);
	double  calSalary() override;
};
// 销售类
class SalesWorker {
protected:
	int  monthlySales;
public:
	SalesWorker(int _monthlySales);
	int getMonthlySales();
};
// 销售员工类
class SalesEmployee : public Employee, public SalesWorker {
public:
	SalesEmployee(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _monthlySales);
	double calSalary() override;
};
// 销售经理类
class SalesManager : public Employee, public SalesWorker {
	// 指向所属销售部门的指针
	SalesDepartment* department;
public:
	SalesManager(std::wstring _departmentID, std::wstring _employeeID, std::wstring _name, int _monthlySales);
	double calSalary() override;
	void setSalesDepartment(SalesDepartment* dep);
	SalesDepartment* getSalesDepartment() const;
};

// 部门基类
class Department {
private:
	// 部门ID
	std::wstring	departmentID;
	// 部门名称
	std::wstring departmentName;
public:
	Department(const std::wstring& id, const std::wstring& name);
	const std::wstring& getDepartmentID() const;
	const std::wstring& getDepartmentName() const;
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
	void setDepartmentTotalWorkHours(int h);
	void addDepartmentTotalWorkHours(int h);

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
	void setSalesDepartmentTotalSales(int s);
	void addSalesDepartmentTotalSales(int s);
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
	void MainLoop();
	void DepartmentLoop();
	void DepartmentAddLoop();
	void DepartmentDeleteLoop();
	void DepartmentUpdateLoop();
	void EmployeeLoop();
	void EmployeeAddLoop();
	void EmployeeDeleteLoop();
	void EmployeeUpdateLoop();
	void StatisticsLoop();
	void StatisticsAllLoop();
	void Exit();
};
