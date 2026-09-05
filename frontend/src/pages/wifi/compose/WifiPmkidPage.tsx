import { NetworksScanner } from "../ui/NetworksScanner";

export const WifiPmkidPage: React.FC = () => {
	return (
		<div className="flex h-full flex-col gap-4 p-4">
			<div className="grid flex-1 grid-cols-1 gap-4 md:grid-cols-[2fr_1fr]">
				<div className="flex flex-col gap-4">
					<NetworksScanner />
				</div>

				<div></div>
			</div>
		</div>
	);
};
