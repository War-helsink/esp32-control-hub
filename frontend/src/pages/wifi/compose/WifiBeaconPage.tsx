import { Background, Header, Trigger } from "@/features/start";
import { useBeacon } from "../hooks/useBeacon";

export const WifiBeaconPage: React.FC = () => {
	const { isActive, isBlocked, isPending, toggleBeacon } = useBeacon();

	return (
		<div className="relative w-full min-h-[calc(100vh-4rem)] flex flex-col justify-between p-4 md:p-10 overflow-hidden bg-background text-foreground">
			<Background isActive={isActive} />
			<div className="relative z-10 w-full max-w-2xl mx-auto flex flex-col flex-1 justify-between gap-8">
				<Header
					isActive={isActive}
					title="Wifi Beacon System"
					description="Creation of many different networks."
				/>
				<Trigger
					isActive={isActive}
					isBlocked={isBlocked}
					isPending={isPending}
					onToggle={toggleBeacon}
				/>
			</div>
		</div>
	);
};
