import { setStatus, type StatusApiResponse } from "@/entities/status";
import { BaseApiService } from "@/shared/services";

export class BleApi extends BaseApiService {
	private static readonly PATH = "/ble";
	private static readonly PATH_SCANNING = `${this.PATH}/scanner`;
	private static readonly PATH_JAMMER = `${this.PATH}/jammer`;

	public static async startScanning() {
		const result = await this.post<StatusApiResponse>(
			`${this.PATH_SCANNING}/start`,
		);
		return setStatus(result);
	}

	public static async stopScanning() {
		const result = await this.post<StatusApiResponse>(
			`${this.PATH_SCANNING}/stop`,
		);
		return setStatus(result);
	}

	public static async startJammer() {
		const result = await this.post<StatusApiResponse>(
			`${this.PATH_JAMMER}/start`,
		);
		return setStatus(result);
	}

	public static async stopJammer() {
		const result = await this.post<StatusApiResponse>(
			`${this.PATH_JAMMER}/stop`,
		);
		return setStatus(result);
	}

	public static async setJammerConfig(power: number) {
		const queryString = new URLSearchParams({
			power: power.toString(),
		}).toString();

		return this.put<{ success: boolean }>(
			`${this.PATH_JAMMER}/config?${queryString}`,
		);
	}
}
