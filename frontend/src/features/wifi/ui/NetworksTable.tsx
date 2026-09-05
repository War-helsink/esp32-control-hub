import { Network } from "lucide-react";

import type { WifiNetwork } from "@/entities/wifi";
import { ScannerEmptyState } from "./ScannerEmptyState";
import {
	Table,
	TableBody,
	TableHead,
	TableHeader,
	TableRow,
} from "@/shared/ui";
import { NetworkTableRow } from "./NetworkTableRow";

interface NetworksTableProps {
	networks: WifiNetwork[];
	selectedNetwork: WifiNetwork | null;
	onSelectNetwork: (network: WifiNetwork) => void;
}

export const NetworksTable: React.FC<NetworksTableProps> = ({
	networks,
	selectedNetwork,
	onSelectNetwork,
}) => {
	if (networks.length === 0) {
		return (
			<ScannerEmptyState
				icon={Network}
				title="No networks discovered"
				description="Start a network scan to discover nearby Wi-Fi access points."
			/>
		);
	}

	return (
		<div className="overflow-x-auto">
			<Table>
				<TableHeader>
					<TableRow className="bg-muted/40 hover:bg-muted/40">
						<TableHead>Network</TableHead>
						<TableHead>BSSID</TableHead>
						<TableHead>Signal</TableHead>
						<TableHead>Channel</TableHead>
						<TableHead className="text-right pr-4">Security</TableHead>
					</TableRow>
				</TableHeader>

				<TableBody>
					{networks.map((network) => (
						<NetworkTableRow
							key={network.bssid}
							network={network}
							isSelected={selectedNetwork?.bssid === network.bssid}
							onSelect={onSelectNetwork}
						/>
					))}
				</TableBody>
			</Table>
		</div>
	);
};
