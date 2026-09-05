import { DeauthControlPanel } from "../ui/DeauthControlPanel";
import { NetworksScanner } from "../ui/NetworksScanner";
import { ClientsScanner } from "../ui/ClientsScanner";

export const WifiDeauthPage: React.FC = () => {
	return (
		<div className="flex h-full flex-col gap-4 p-4">
			<div className="grid flex-1 grid-cols-1 gap-4 md:grid-cols-[2fr_1fr]">
				<div className="flex flex-col gap-4">
					<NetworksScanner />
					<ClientsScanner />
				</div>

				<div>
					<DeauthControlPanel />
				</div>
			</div>
		</div>
	);
};
