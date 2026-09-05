import { toast } from "sonner";
import saveAs from "file-saver";
import { FileClock, Download, FileCodeIcon } from "lucide-react";
import { WifiApi, type DownloadHandshakeResult } from "@/entities/wifi";
import {
	Spinner,
	Attachment,
	AttachmentTitle,
	AttachmentActions,
	AttachmentMedia,
	AttachmentContent,
	AttachmentDescription,
	AttachmentAction,
} from "@/shared/ui";
import { useEffect, useState } from "react";
import { eventSourceService } from "@/shared/services";

export type HandshakeStatus = "idle" | "uploading" | "processing";

interface HandshakeAttachmentProps {
	isActive: boolean;
}

export const HandshakeAttachment: React.FC<HandshakeAttachmentProps> = ({
	isActive,
}) => {
	const [isReady, setIsReady] = useState(false);

	useEffect(() => {
		return eventSourceService.subscribe<{
			isReady: boolean;
		}>("wifi:handshake:status", (event) => {
			setIsReady(event.isReady);
		});
	}, []);

	async function handleDownload() {
		const result = await WifiApi.downloadHccapx().catch(() => {
			toast.warning("File is not ready yet");
			return {
				success: false,
				error: "File is not ready yet",
			} as DownloadHandshakeResult;
		});
		if (!result.success) {
			toast.warning("File is not ready yet");
			return;
		}
		saveAs(result.blob, "ready.hccapx");
		toast.success("Handshake downloaded");
	}

	const status: HandshakeStatus = isReady
		? "uploading"
		: isActive
			? "processing"
			: "idle";

	if (status === "idle") {
		return (
			<Attachment state={status} className="w-full">
				<AttachmentMedia>
					<FileClock />
				</AttachmentMedia>
				<AttachmentContent>
					<AttachmentTitle>File</AttachmentTitle>
					<AttachmentDescription>Not ready</AttachmentDescription>
				</AttachmentContent>
			</Attachment>
		);
	}

	if (status === "processing") {
		return (
			<Attachment state={status} className="w-full">
				<AttachmentMedia>
					<Spinner />
				</AttachmentMedia>
				<AttachmentContent>
					<AttachmentTitle>Formation</AttachmentTitle>
					<AttachmentDescription>
						Listening for EAPOL packets...
					</AttachmentDescription>
				</AttachmentContent>
			</Attachment>
		);
	}

	return (
		<Attachment state={status} className="w-full">
			<AttachmentMedia>
				<FileCodeIcon />
			</AttachmentMedia>
			<AttachmentContent>
				<AttachmentTitle>ready.hccapx</AttachmentTitle>
				<AttachmentDescription>393 B • Hashcat hccapx</AttachmentDescription>
			</AttachmentContent>
			<AttachmentActions>
				<AttachmentAction aria-label="Cancel upload" onClick={handleDownload}>
					<Download />
				</AttachmentAction>
			</AttachmentActions>
		</Attachment>
	);
};
