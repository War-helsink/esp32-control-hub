import { setStatus, type StatusApiResponse } from "@/entities/status";
import { BaseApiService } from "@/shared/services";
import type {
	DownloadHandshakeResult,
	HandshakeType,
	WifiClient,
	WifiNetwork,
} from "../types";

export class WifiApi extends BaseApiService {
	private static readonly PATH = "/wifi";
	private static readonly PATH_SCANNER = `${this.PATH}/scanner`;
	private static readonly PATH_ATTACK = `${this.PATH}/attack`;

	// ===== Network =====
	static async startScanner() {
		const result = await this.post<StatusApiResponse>(
			`${WifiApi.PATH_SCANNER}/start`,
		);

		return setStatus(result);
	}

	static async stopScanner() {
		const result = await this.post<StatusApiResponse>(
			`${WifiApi.PATH_SCANNER}/stop`,
		);

		return setStatus(result);
	}

	// Select
	static async selectNetwork(params: WifiNetwork) {
		return await this.post<StatusApiResponse>(
			`${WifiApi.PATH}/network/select`,
			params,
		);
	}

	static async clearSelectedNetwork() {
		return await this.post<StatusApiResponse>(
			`${WifiApi.PATH}/network/select/clear`,
		);
	}

	static async selectClient(params: WifiClient) {
		return await this.post<StatusApiResponse>(
			`${WifiApi.PATH}/client/select`,
			params,
		);
	}

	static async clearSelectedClient() {
		return await this.post<StatusApiResponse>(
			`${WifiApi.PATH}/client/select/clear`,
		);
	}

	// ===== Deauth =====
	static async startDeauth() {
		const result = await this.post<StatusApiResponse>(
			`${WifiApi.PATH_ATTACK}/deauth/start`,
		);
		return setStatus(result);
	}

	static async stopDeauth() {
		const result = await this.post<StatusApiResponse>(
			`${WifiApi.PATH_ATTACK}/deauth/stop`,
		);
		return setStatus(result);
	}

	// ===== Beacon =====
	static async startBeacon(channel: number) {
		const result = await this.post<StatusApiResponse>(
			`${WifiApi.PATH_ATTACK}/beacon/start`,
			{
				channel,
			},
		);
		return setStatus(result);
	}

	static async stopBeacon() {
		const result = await this.post<StatusApiResponse>(
			`${WifiApi.PATH_ATTACK}/beacon/stop`,
		);
		return setStatus(result);
	}

	// ===== Handshake =====
	static async startHandshake(params: { ssid: string; type: HandshakeType }) {
		const result = await this.post<StatusApiResponse>(
			`${WifiApi.PATH_ATTACK}/handshake/start`,
			params,
		);
		return setStatus(result);
	}

	static async stopHandshake() {
		const result = await this.post<StatusApiResponse>(
			`${WifiApi.PATH_ATTACK}/handshake/stop`,
		);
		return setStatus(result);
	}

	static async downloadHccapx(): Promise<DownloadHandshakeResult> {
		try {
			const response = await this.request(
				`${WifiApi.PATH_ATTACK}/handshake/hccapx/download`,
				{
					method: "POST",
				},
			);

			const contentType = response.headers.get("content-type");

			if (contentType?.includes("application/json")) {
				const result = (await response.json()) as {
					success: false;
					error: string;
				};

				return result;
			}

			if (!response.ok) {
				return {
					success: false,
					error: "download_failed",
				};
			}

			return {
				success: true,
				blob: await response.blob(),
			};
		} catch {
			return {
				success: false,
				error: "network_error",
			};
		}
	}

	static async downloadPcap(): Promise<DownloadHandshakeResult> {
		try {
			const response = await this.request(
				`${WifiApi.PATH_ATTACK}/handshake/pcap/download`,
				{
					method: "POST",
				},
			);

			const contentType = response.headers.get("content-type");

			if (contentType?.includes("application/json")) {
				const result = (await response.json()) as {
					success: false;
					error: string;
				};

				return result;
			}

			if (!response.ok) {
				return {
					success: false,
					error: "download_failed",
				};
			}

			return {
				success: true,
				blob: await response.blob(),
			};
		} catch {
			return {
				success: false,
				error: "network_error",
			};
		}
	}
}
