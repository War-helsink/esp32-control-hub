import { useWifiStore } from "@/entities/wifi";
import {
	Button,
	Card,
	CardContent,
	CardFooter,
	CardHeader,
	CardTitle,
	Field,
	FieldLabel,
	Input,
} from "@/shared/ui";
import { useDeauthAttack } from "../hooks/useDeauthAttack";

export const DeauthControlPanel: React.FC = () => {
	const selectedNetwork = useWifiStore((s) => s.selectedNetwork);
	const selectedClient = useWifiStore((s) => s.selectedClient);

	const { isActive, isPending, isBlocked, toggleDeauthAttack } =
		useDeauthAttack();

	return (
		<Card>
			<CardHeader className="flex flex-row items-center justify-between">
				<CardTitle className="text-xl">Deauthentication</CardTitle>
			</CardHeader>

			<CardContent className="flex flex-col gap-4">
				<Field>
					<FieldLabel htmlFor="input-demo-api-key">Target Network</FieldLabel>
					<Input
						disabled
						id="input-demo-api-key"
						placeholder="Not selected"
						value={selectedNetwork ? selectedNetwork.bssid : ""}
					/>
				</Field>
				<Field>
					<FieldLabel htmlFor="input-demo-api-key">Client</FieldLabel>
					<Input
						disabled
						id="input-demo-api-key"
						placeholder="Not selected"
						value={selectedClient ? selectedClient.mac : ""}
					/>
				</Field>
			</CardContent>
			<CardFooter className="justify-between border-t bg-muted/20 py-3 text-xs text-muted-foreground">
				<Button
					disabled={isPending || isBlocked}
					variant={isActive ? "destructive" : "default"}
					onClick={toggleDeauthAttack}
				>
					{isActive ? "Stop the attack" : "Launch an attack"}
				</Button>
			</CardFooter>
		</Card>
	);
};
