#pragma once

constexpr auto TASK_MATCH_START = 1337;

namespace SurfMod {
	typedef struct S_TASK_INFO {
		int				Index;
		float			Time;
		float			EndTime;
		bool			Loop;
		bool			Free;
		void*			FunctionCallback;
		int				FunctionParameter;
	} P_TASK_INFO, * LP_TASK_INFO;

	class Task {
	public:
		void ServerActivate();
		void ServerDeactivate();

		void Create(int Index, float Time, bool Loop, void* FunctionCallback, int FunctionParameter);
		void Remove(int Index);
		void ServerFrame();

	private:
		std::unordered_map<int, P_TASK_INFO> m_Data;
	};
};

extern SurfMod::Task gSurfModTask;