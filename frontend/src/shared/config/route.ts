class Routes {
	//Dashboard
	readonly dashboard = "/" as const;

	//Bluetooth
	private bluetooth = "/bluetooth" as const;
	readonly bluetoothScanner = `${this.bluetooth}/scanner` as const;
	readonly bluetoothJammer = `${this.bluetooth}/jammer` as const;

	// Wifi
	private wifi = "/wifi" as const;
	readonly wifiScanner = `${this.wifi}/scanner` as const;
	private wifiAttacks = `${this.wifi}/attacks` as const;
	readonly wifiAttackDeauth = `${this.wifiAttacks}/deauth` as const;
	readonly wifiAttackBeacon = `${this.wifiAttacks}/beacon` as const;
	readonly wifiAttackHandshake = `${this.wifiAttacks}/handshake` as const;
	readonly wifiAttackPmkid = `${this.wifiAttacks}/pmkid` as const;

	// RF / Sub-GHz
	// private rf = "/rf" as const;
}

export const routesConfig = new Routes();
