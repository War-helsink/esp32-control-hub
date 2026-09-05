export function getSignalLevel(rssi: number): 1 | 2 | 3 | 4 {
	if (rssi >= -50) return 4;
	if (rssi >= -65) return 3;
	if (rssi >= -75) return 2;

	return 1;
}

export function getSignalLabel(rssi: number): string {
	if (rssi >= -50) return "Excellent";
	if (rssi >= -65) return "Good";
	if (rssi >= -75) return "Fair";

	return "Weak";
}

export function getSignalTextClass(rssi: number): string {
	if (rssi >= -50) return "text-emerald-600";
	if (rssi >= -65) return "text-sky-600";
	if (rssi >= -75) return "text-amber-600";

	return "text-destructive";
}

export function formatEncryption(encryption: string): string {
	if (!encryption || encryption.toLowerCase() === "open") {
		return "Open";
	}

	return encryption;
}

export function formatMacAddress(mac: string): string {
	return mac.toUpperCase();
}
