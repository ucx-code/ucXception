import React, { useContext } from "react";
import AlertsContext from "../context/AlertsProvider";
import { CAlert } from "@coreui/react";
import CIcon from "@coreui/icons-react";

const AppAlerts = () => {
  const { alert } = useContext(AlertsContext);

  return (
    <>
      {Object.keys(alert).length !== 0 ? (
        <CAlert color={alert["color"]} className="d-flex align-items-center">
          <CIcon
            icon={alert["icon"]}
            className="flex-shrink-0 me-2"
            width={24}
            height={24}
          />
          <div>{alert["message"]}</div>
        </CAlert>
      ) : null}
    </>
  );
};

export default React.memo(AppAlerts);
