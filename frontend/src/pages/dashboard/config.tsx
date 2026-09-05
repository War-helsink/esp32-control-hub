import { Radio, ShieldAlert, FileKey, Settings } from "lucide-react";
import type { FeatureModule } from "./types";

export const features: FeatureModule[] = [
	{
		id: "sniffer",
		title: "Promiscuous Sniffer (802.11)",
		description: "Capture raw 802.11 Wi-Fi frames on the 2.4 GHz spectrum.",
		extendedDoc:
			"Utilizes the built-in esp_wifi_set_promiscuous() function. Allows parsing MAC-layer headers (Management, Data, Control). All processing is offloaded to Core 1 in FreeRTOS to avoid blocking the Web UI running on Core 0.",
		status: "ready",
		icon: <Radio className="h-5 w-5 text-blue-500" />,
	},
	{
		id: "deauth",
		title: "Deauthentication Engine",
		description:
			"Transmit Management frames (0xC0) to disconnect specific targets.",
		extendedDoc:
			"Generates spoofed deauthentication packets acting as the Access Point (AP) to the client (STA). Forces devices to reconnect to capture the subsequent WPA handshake. Supports target whitelist/blacklist queues.",
		status: "ready",
		icon: <ShieldAlert className="h-5 w-5 text-destructive" />,
	},
	{
		id: "handshake",
		title: "Handshake & PMKID Capture",
		description:
			"Extract EAPOL authentication packets from raw 802.11 data streams.",
		extendedDoc:
			"Filters data traffic for the 802.1X authentication flag (0x888E). Once all 4 stages of the 4-Way Handshake or a valid PMKID are caught, it encapsulates them into a PCAP structure and writes it directly onto the LittleFS flash storage.",
		status: "in_development",
		icon: <FileKey className="h-5 w-5 text-amber-500" />,
	},
	{
		id: "web-ui",
		title: "Async Web Server & UI Engine",
		description:
			"Control and monitor the ESP32-S3 hardware via an interactive real-time dashboard.",
		extendedDoc:
			"Powered by ESPAsyncWebServer. Serves the production React build straight from LittleFS. Uses Server-Sent Events (SSE / EventSource) to stream logs and chip telemetry without overhead, heavily preserving ESP32 CPU cycles.",
		status: "planned",
		icon: <Settings className="h-5 w-5 text-green-500" />,
	},
];
