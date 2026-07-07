import { SidebarInset, SidebarTrigger, Separator } from "@/shared/ui";

interface UserSidebarProps extends React.PropsWithChildren {
	header?: React.ReactNode | undefined;
}

export const AppSidebarInset: React.FC<UserSidebarProps> = ({
	children,
	header,
}) => {
	return (
		<SidebarInset>
			<header className="flex h-14 shrink-0 items-center border-b gap-2">
				<div className="flex items-center gap-2 px-4">
					<SidebarTrigger className="-ml-1" />
					<Separator orientation="vertical" className="mr-2! h-4!" />
				</div>
				<div className="grow h-full">{header}</div>
			</header>
			<div className="grow overflow-y-auto">{children}</div>
		</SidebarInset>
	);
};
