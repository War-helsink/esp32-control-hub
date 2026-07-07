import { BaseApiService } from "@/shared/services";

export interface JammerStatusResponse {
	active: boolean;
	power: number;
}

export class BleApi extends BaseApiService {
	private static readonly PATH = "/ble";
	private static readonly PATH_SCANNING = `${this.PATH}/scanner`;
	private static readonly PATH_JAMMER = `${this.PATH}/jammer`;

	public static async startScanning(): Promise<void> {
		return this.post<void>(`${this.PATH_SCANNING}/start`);
	}

	public static async stopScanning(): Promise<void> {
		return this.post<void>(`${this.PATH_SCANNING}/stop`);
	}

	public static async getScanningStatus(): Promise<{ scanning: boolean }> {
		return this.get<{ scanning: boolean }>(`${this.PATH_SCANNING}/status`);
	}

	public static async startJammer(): Promise<void> {
		return this.post<void>(`${this.PATH_JAMMER}/start`);
	}

	public static async stopJammer(): Promise<void> {
		return this.post<void>(`${this.PATH_JAMMER}/stop`);
	}

	public static async getJammerStatus(): Promise<JammerStatusResponse> {
		return this.get<JammerStatusResponse>(`${this.PATH_JAMMER}/status`);
	}

	public static async setJammerConfig(power: number): Promise<void> {
		return this.put<void>(`${this.PATH_JAMMER}/config?power=${power}`);
	}
}
