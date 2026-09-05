import { Bluetooth, Radio, Router } from "lucide-react";
import type { ActiveProcess } from "@/entities/status";

export function getActionLabel(process: ActiveProcess) {
	switch (process.module) {
		case "wifi": {
			switch (process.action) {
				case "scan":
					return "Scan";

				case "deauth":
					return "Deauthentication";

				case "beacon":
					return "Beacon";

				case "handshake":
					return "Handshake";
			}
			break;
		}
		case "bluetooth": {
			switch (process.action) {
				case "scan":
					return "Device scan";

				case "jammer":
					return "Jammer";
			}
			break;
		}
		default:
			return "Unknown operation";
	}
}

export function getModuleIcon(module: ActiveProcess["module"]) {
	switch (module) {
		case "wifi":
			return Router;

		case "bluetooth":
			return Bluetooth;

		default:
			return Radio;
	}
}
