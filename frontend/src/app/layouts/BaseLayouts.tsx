import { Outlet } from "react-router";
import { AppSidebar, AppSidebarInset } from "@/widgets/sidebar";
import { SidebarProvider } from "@/shared/ui";

const BaseLayouts: React.FC = () => {
	return (
		<SidebarProvider>
			<AppSidebar />
			<AppSidebarInset>
				<Outlet />
			</AppSidebarInset>
		</SidebarProvider>
	);
};

export default BaseLayouts;
