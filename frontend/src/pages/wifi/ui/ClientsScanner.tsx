import { ScannerCardHeader, ClientsTable } from "@/features/wifi";
import { useClients, useWifiStore, useSelectClient } from "@/entities/wifi";
import { Card, CardContent, CardFooter } from "@/shared/ui";

export const ClientsScanner: React.FC = () => {
	const clients = useClients();
	const selectedClient = useWifiStore((store) => store.selectedClient);
	const { selectClient } = useSelectClient();

	return (
		<Card className="overflow-hidden shadow-sm gap-0">
			<ScannerCardHeader title="Connected Clients" />

			<CardContent className="p-0">
				<ClientsTable
					clients={clients}
					selectedClient={selectedClient}
					onSelectClient={selectClient}
				/>
			</CardContent>

			<CardFooter className="justify-between border-t bg-muted/20 py-3 text-xs text-muted-foreground">
				<span>Showing {clients.length} clients</span>
			</CardFooter>
		</Card>
	);
};
