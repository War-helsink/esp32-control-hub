import { create } from "zustand";
import type { StatusStore, StatusState, ActiveProcess } from "./types";

const initialState: StatusState = {
	status: "normal",
	activeProcess: null,
};

export const useStatusStore = create<StatusStore>((set, get) => ({
	...initialState,
	startRunning: (process: ActiveProcess) => {
		const status = get().status;
		if (status === "normal") {
			set({
				status: "running",
				activeProcess: process,
			});
		}
	},
	setNormal: () => set({ ...initialState }),
}));
