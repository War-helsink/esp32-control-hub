import { useBleJammer } from "../hooks/useBleJammer";
import { JammerBackground } from "../ui/JammerBackground";
import { JammerHeader } from "../ui/JammerHeader";
import { JammerPowerControl } from "../ui/JammerPowerControl";
import { JammerTrigger } from "../ui/JammerTrigger";

export const BluetoothJammerPage: React.FC = () => {
	const { isActive, power, isLoading, toggleJammer, handlePowerChange } =
		useBleJammer();

	return (
		<div className="relative w-full min-h-[calc(100vh-4rem)] flex flex-col justify-between p-6 md:p-10 overflow-hidden bg-background text-foreground">
			<JammerBackground isActive={isActive} />
			<div className="relative z-10 w-full max-w-2xl mx-auto flex flex-col flex-1 justify-between gap-8">
				<JammerHeader isActive={isActive} />
				<JammerTrigger
					isActive={isActive}
					isLoading={isLoading}
					onToggle={toggleJammer}
				/>
				<JammerPowerControl
					power={power}
					disabled={isLoading}
					onPowerChange={handlePowerChange}
				/>
			</div>
		</div>
	);
};
