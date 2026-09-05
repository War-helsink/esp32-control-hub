export interface WifiNetwork {
	bssid: string;
	ssid: string;
	channel: number;
	rssi: number;
	encryption: string;
}

export type HandshakeType = "passive" | "deauth" | "broadcast";

export type DownloadHandshakeResult =
	| {
			success: true;
			blob: Blob;
	  }
	| {
			success: false;
			error: string;
	  };

export interface WifiClient {
	mac: string;
	bssid: string;
	rssi: number;
}

export interface WifiNetworkFoundEvent {
	payload: WifiNetwork;
}

export interface WifiClientFoundEvent {
	payload: WifiClient;
}

export interface WifiState {
	// Network
	networksByBssid: Record<string, WifiNetwork>;
	selectedNetwork: WifiNetwork | null;

	// Client
	clientsByMac: Record<string, WifiClient>;
	selectedClient: WifiClient | null;
}

export interface WifiActions {
	// Network
	selectNetwork: (network: WifiNetwork | null) => void;
	upsertNetwork: (network: WifiNetwork) => void;
	clearNetworks: () => void;

	// Client
	upsertClient: (client: WifiClient) => void;
	selectClient: (client: WifiClient | null) => void;
	clearClient: () => void;

	// Reset
	restoreSelection: (state: WifiState) => void;
	resetSelection: () => void;
	reset: () => void;
}

export type WifiStore = WifiActions & WifiState;
