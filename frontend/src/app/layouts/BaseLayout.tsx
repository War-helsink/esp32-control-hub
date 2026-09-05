import { AppSidebar, AppSidebarInset } from "@/features/sidebar";
import { DeviceStatus } from "@/features/device";
import { StatusLayout } from "@/entities/status";
import { SidebarProvider, TooltipProvider } from "@/shared/ui";

export const BaseLayout: React.FC = () => {
	return (
		<SidebarProvider>
			<AppSidebar />
			<AppSidebarInset
				header={
					<TooltipProvider>
						<DeviceStatus />
					</TooltipProvider>
				}
			>
				<StatusLayout />
			</AppSidebarInset>
		</SidebarProvider>
	);
};
