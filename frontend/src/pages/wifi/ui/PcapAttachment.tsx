import { toast } from "sonner";
import saveAs from "file-saver";
import { FileClock, Download, FileCodeIcon } from "lucide-react";
import { WifiApi, type DownloadHandshakeResult } from "@/entities/wifi";
import {
	Attachment,
	AttachmentTitle,
	AttachmentActions,
	AttachmentMedia,
	AttachmentContent,
	AttachmentDescription,
	AttachmentAction,
} from "@/shared/ui";

export type HandshakeStatus = "idle" | "uploading" | "processing";

interface PcapAttachmentProps {
	isActive: boolean;
}

export const PcapAttachment: React.FC<PcapAttachmentProps> = ({ isActive }) => {
	async function handleDownload() {
		const result = await WifiApi.downloadPcap().catch(() => {
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
		saveAs(result.blob, "ready.pcap");
		toast.success("Pcap downloaded");
	}

	if (isActive) {
		return (
			<Attachment state="uploading" className="w-full">
				<AttachmentMedia>
					<FileCodeIcon />
				</AttachmentMedia>
				<AttachmentContent>
					<AttachmentTitle>ready.pcap</AttachmentTitle>
					<AttachmentDescription>File Pcap</AttachmentDescription>
				</AttachmentContent>
				<AttachmentActions>
					<AttachmentAction aria-label="Cancel upload" onClick={handleDownload}>
						<Download />
					</AttachmentAction>
				</AttachmentActions>
			</Attachment>
		);
	}

	return (
		<Attachment state="done" className="w-full">
			<AttachmentMedia>
				<FileClock />
			</AttachmentMedia>
			<AttachmentContent>
				<AttachmentTitle>File Pcap</AttachmentTitle>
				<AttachmentDescription>There is no file</AttachmentDescription>
			</AttachmentContent>
		</Attachment>
	);
};
