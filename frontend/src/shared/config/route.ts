class Routes {
	//Dashboard
	readonly dashboard = "/" as const;

	//Bluetooth
	private bluetooth = "/bluetooth" as const;
	readonly bluetoothScanner = `${this.bluetooth}/scanner` as const;
	readonly bluetoothJammer = `${this.bluetooth}/jammer` as const;

	// Wifi
	// private wifi = "/rf" as const;

	// RF / Sub-GHz
	// private rf = "/rf" as const;
}

export const routesConfig = new Routes();
