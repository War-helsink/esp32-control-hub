import {
	BluetoothSearching,
	Handshake,
	KeyRound,
	LayoutList,
	Radar,
	RadioOff,
	WifiCog,
	ZapOff,
} from "lucide-react";
import { routesConfig } from "@/shared/config";
import type { NavMenu } from "./types";

export const navMain: NavMenu[] = [
	{
		title: "Dashboard",
		url: routesConfig.dashboard,
		icon: LayoutList,
	},
	{
		title: "Bluetooth",
		items: [
			{
				title: "Scanner",
				url: routesConfig.bluetoothScanner,
				icon: BluetoothSearching,
			},
			{
				title: "Jammer",
				url: routesConfig.bluetoothJammer,
				icon: RadioOff,
			},
		],
	},
	{
		title: "Wifi",
		items: [
			{
				title: "Scanner",
				url: routesConfig.wifiScanner,
				icon: Radar,
			},
			{
				title: "Deauth",
				url: routesConfig.wifiAttackDeauth,
				icon: ZapOff,
			},
			{
				title: "Beacon",
				url: routesConfig.wifiAttackBeacon,
				icon: WifiCog,
			},
			{
				title: "Handshake Capture",
				url: routesConfig.wifiAttackHandshake,
				icon: Handshake,
			},
			{
				title: "PMKID Capture",
				url: routesConfig.wifiAttackPmkid,
				icon: KeyRound,
			},
		],
	},
	{
		title: "Radio Frequency",
		items: [],
	},
];
