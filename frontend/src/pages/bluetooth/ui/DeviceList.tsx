import { Loader2 } from "lucide-react";
import type { BleDevice } from "@/entities/bluetooth";
import { Card, CardContent, CardHeader, CardTitle } from "@/shared/ui";
import { DeviceRow } from "./DeviceRow";

interface DeviceListProps {
	devices: BleDevice[];
	isScanning: boolean;
	hasDevices: boolean;
}

export const DeviceList: React.FC<DeviceListProps> = ({
	devices,
	isScanning,
	hasDevices,
}) => (
	<Card>
		<CardHeader>
			<CardTitle>Detected devices</CardTitle>
		</CardHeader>
		<CardContent>
			{!isScanning && !hasDevices && (
				<div className="rounded-lg border border-dashed p-6 text-center text-sm text-muted-foreground">
					Scanning has not started
				</div>
			)}

			{isScanning && !hasDevices && (
				<div className="flex items-center justify-center gap-2 rounded-lg border border-dashed p-6 text-sm text-muted-foreground">
					<Loader2 className="h-4 w-4 animate-spin" />
					Waiting for nearby Bluetooth devices...
				</div>
			)}

			{hasDevices && (
				<div className="flex flex-col gap-3">
					{devices.map((device) => (
						<DeviceRow key={device.address} device={device} />
					))}
				</div>
			)}
		</CardContent>
	</Card>
);
