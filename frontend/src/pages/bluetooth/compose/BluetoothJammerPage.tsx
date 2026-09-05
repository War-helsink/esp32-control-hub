import { Background, Header, Trigger } from "@/features/start";
import { useJammer } from "../hooks/useJammer";
import { JammerPowerControl } from "../ui/JammerPowerControl";

export const BluetoothJammerPage: React.FC = () => {
	const { isActive, isBlocked, isPending, power, toggleJammer, powerChange } =
		useJammer();

	return (
		<div className="relative w-full min-h-[calc(100vh-4rem)] flex flex-col justify-between p-6 md:p-10 overflow-hidden bg-background text-foreground">
			<Background isActive={isActive} />
			<div className="relative z-10 w-full max-w-2xl mx-auto flex flex-col flex-1 justify-between gap-8">
				<Header
					isActive={isActive}
					title="BLE Jammer System"
					description="Manage Bluetooth protocol suppression and transmission power."
				/>
				<Trigger
					isActive={isActive}
					isBlocked={isBlocked}
					isPending={isPending}
					onToggle={toggleJammer}
				/>
				<JammerPowerControl
					power={power}
					disabled={isPending}
					onPowerChange={powerChange}
				/>
			</div>
		</div>
	);
};
