import { create } from "zustand";
import { persist } from "zustand/middleware";
import type { WifiStore, WifiState } from "./types";

const initialState: WifiState = {
	// Network
	networksByBssid: {},
	selectedNetwork: null,

	// Client
	clientsByMac: {},
	selectedClient: null,
};

export const useWifiStore = create(
	persist<WifiStore>(
		(set) => ({
			...initialState,

			// Network
			selectNetwork: (selectedNetwork) =>
				set(() => ({
					selectedNetwork,
					selectedClient: null,
					clientsByMac: {},
				})),
			upsertNetwork: (network) =>
				set((state) => ({
					networksByBssid: {
						...state.networksByBssid,
						[network.bssid]: { ...network },
					},
				})),
			clearNetworks: () =>
				set({
					networksByBssid: {},
					clientsByMac: {},
					selectedNetwork: null,
					selectedClient: null,
				}),

			// Client
			upsertClient: (client) =>
				set((state) => ({
					clientsByMac: {
						...state.clientsByMac,
						[client.mac]: { ...client },
					},
				})),
			selectClient: (selectedClient) =>
				set(() => ({
					selectedClient,
				})),
			clearClient: () => set({ clientsByMac: {}, selectedClient: null }),

			// Reset
			restoreSelection: (state) => set(state),
			resetSelection: () =>
				set({ selectedNetwork: null, selectedClient: null, clientsByMac: {} }),
			reset: () => set(initialState),
		}),
		{
			name: "wifi",
		},
	),
);
