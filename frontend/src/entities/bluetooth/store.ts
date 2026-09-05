import { create } from "zustand";
import type { BleState, BleStore } from "./types";

const initialState: BleState = {
	devicesByAddress: {},
	selectedDevice: null,
};

export const useBleStore = create<BleStore>((set) => ({
	...initialState,

	selectDevice: (selectedDevice) => {
		return set({
			selectedDevice,
		});
	},
	upsertDevice: (device) =>
		set((state) => ({
			devicesByAddress: { ...state.devicesByAddress, [device.address]: device },
		})),
	clearDevices: () => set({ devicesByAddress: {} }),

	resetSelection: () => set({ selectedDevice: null }),
	reset: () => set(initialState),
}));
