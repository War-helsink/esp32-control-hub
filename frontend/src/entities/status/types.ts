export type StatusApiResponse =
	| ({
			success: true;
	  } & StatusState)
	| ({
			success: false;
			error: string;
	  } & StatusState);

export type StatusEvent = Omit<ActiveProcess, "startedAt">;

export type ActiveProcess =
	| {
			module: "wifi";
			action: "scan" | "deauth" | "handshake" | "beacon";
	  }
	| {
			module: "bluetooth";
			action: "scan" | "jammer";
	  };

export type StatusState =
	| {
			status: "normal";
			activeProcess: null;
	  }
	| {
			status: "running";
			activeProcess: ActiveProcess;
	  };

export interface StatusActions {
	startRunning: (process: ActiveProcess) => void;
	setNormal: () => void;
}

export type StatusStore = StatusActions & StatusState;
