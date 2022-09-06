import { createContext, useState } from "react";

const AlertsContext = createContext({});

export const AlertsProvider = ({ children }) => {
  const [alert, setAlert] = useState({});

  function addAlert(message, color, icon) {
    setAlert({ message: message, color: color, icon: icon });
  }

  return (
    <AlertsContext.Provider
      value={{
        alert,
        setAlert,
        addAlert,
      }}
    >
      {children}
    </AlertsContext.Provider>
  );
};

export default AlertsContext;
