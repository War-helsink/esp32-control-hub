import { NetworksScanner } from "../ui/NetworksScanner";
import { ClientsScanner } from "../ui/ClientsScanner";

export const WifiScannerPage: React.FC = () => {
	return (
		<div className="h-full flex flex-col gap-5 p-4">
			<NetworksScanner />
			<ClientsScanner />
		</div>
	);
};
