import { useStatusStore } from "./store";
import type { ActiveProcess, StatusState, StatusStore } from "./types";

export function setStatus(statusState: StatusState) {
	if (statusState.status === "normal") {
		useStatusStore.getState().setNormal();
		return statusState;
	}
	useStatusStore.getState().startRunning(statusState.activeProcess);
	return statusState;
}

export function isRunning(
	status: StatusStore,
	module: ActiveProcess["module"],
	action: ActiveProcess["action"],
) {
	return (
		status.status === "running" &&
		status.activeProcess.module === module &&
		status.activeProcess.action === action
	);
}

export function canStart(
	status: StatusStore,
	module: ActiveProcess["module"],
	action: ActiveProcess["action"],
) {
	return status.status === "normal" || isRunning(status, module, action);
}
