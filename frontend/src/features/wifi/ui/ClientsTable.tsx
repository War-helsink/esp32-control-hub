import { Users } from "lucide-react";

import type { WifiClient } from "@/entities/wifi";
import { ScannerEmptyState } from "./ScannerEmptyState";
import {
	Table,
	TableBody,
	TableHead,
	TableHeader,
	TableRow,
} from "@/shared/ui";
import { ClientTableRow } from "./ClientTableRow";

interface ClientsTableProps {
	clients: WifiClient[];
	selectedClient: WifiClient | null;
	onSelectClient: (client: WifiClient | null) => void;
}

export const ClientsTable: React.FC<ClientsTableProps> = ({
	clients,
	selectedClient,
	onSelectClient,
}) => {
	if (clients.length === 0) {
		return (
			<ScannerEmptyState
				icon={Users}
				title="No clients discovered"
				description="Start the client scanner to observe devices associated with the selected access point."
			/>
		);
	}

	return (
		<div className="overflow-x-auto">
			<Table>
				<TableHeader>
					<TableRow className="bg-muted/40 hover:bg-muted/40">
						<TableHead>Client MAC</TableHead>
						<TableHead>Signal</TableHead>
						<TableHead className="text-right pr-4">Connected AP</TableHead>
					</TableRow>
				</TableHeader>

				<TableBody>
					{clients.map((client) => (
						<ClientTableRow
							key={`${client.mac}-${client.bssid}`}
							client={client}
							isSelected={client.mac === selectedClient?.mac}
							onSelect={onSelectClient}
						/>
					))}
				</TableBody>
			</Table>
		</div>
	);
};
