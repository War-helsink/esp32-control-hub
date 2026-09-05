import { useWifiStore, type HandshakeType } from "@/entities/wifi";
import {
	Button,
	Card,
	CardContent,
	CardFooter,
	CardHeader,
	CardTitle,
	Field,
	FieldContent,
	FieldDescription,
	FieldLabel,
	FieldTitle,
	Input,
	RadioGroup,
	RadioGroupItem,
} from "@/shared/ui";
import { useHandshake } from "../hooks/useHandshake";
import { HandshakeAttachment } from "./HandshakeAttachment";
import { PcapAttachment } from "./PcapAttachment";

export const HandshakeControlPanel: React.FC = () => {
	const {
		type,
		changeType,
		isActive,
		isBlocked,
		isPending,
		toggleHandshakeAttack,
	} = useHandshake();
	const selectedNetwork = useWifiStore((s) => s.selectedNetwork);

	return (
		<Card>
			<CardHeader className="flex flex-row items-center justify-between">
				<CardTitle className="text-xl">Handshake</CardTitle>
			</CardHeader>

			<CardContent className="flex flex-col gap-4">
				<HandshakeAttachment isActive={isActive} />
				<PcapAttachment isActive={isActive} />
				<Field>
					<FieldLabel htmlFor="input-demo-api-key">Target Network</FieldLabel>
					<Input
						disabled
						id="input-demo-api-key"
						placeholder="Not selected"
						value={selectedNetwork ? selectedNetwork.bssid : ""}
					/>
				</Field>
				<RadioGroup
					className="max-w-sm"
					value={type}
					onValueChange={(value: HandshakeType) => changeType(value)}
				>
					<FieldLabel htmlFor="r1" className="cursor-pointer">
						<Field orientation="horizontal">
							<FieldContent>
								<FieldTitle>Passive</FieldTitle>
								<FieldDescription>
									Passive listening to the airwaves.
								</FieldDescription>
							</FieldContent>
							<RadioGroupItem value="passive" id="r1" disabled={isActive} />
						</Field>
					</FieldLabel>
					<FieldLabel htmlFor="r2" className="cursor-pointer">
						<Field orientation="horizontal">
							<FieldContent>
								<FieldTitle>Deauth rogue ap</FieldTitle>
								<FieldDescription>
									In conjunction with a deauthentication attack.
								</FieldDescription>
							</FieldContent>
							<RadioGroupItem value="deauth" id="r2" disabled={isActive} />
						</Field>
					</FieldLabel>
					<FieldLabel htmlFor="r3" className="cursor-pointer">
						<Field orientation="horizontal" data-disabled>
							<FieldContent>
								<FieldTitle>Broadcast</FieldTitle>
								<FieldDescription>
									Creating a fake access point.
								</FieldDescription>
							</FieldContent>
							<RadioGroupItem value="broadcast" id="r3" disabled />
						</Field>
					</FieldLabel>
				</RadioGroup>
			</CardContent>
			<CardFooter className="justify-between border-t bg-muted/20 py-3 text-xs text-muted-foreground">
				<Button
					disabled={isPending || isBlocked}
					variant={isActive ? "destructive" : "default"}
					onClick={toggleHandshakeAttack}
				>
					{isActive ? "Stop the attack" : "Launch an attack"}
				</Button>
			</CardFooter>
		</Card>
	);
};
