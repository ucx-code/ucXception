import { createContext, useState } from "react";

const AuthContext = createContext({});

export const AuthProvider = ({ children }) => {
  const [isAuth, setAuth] = useState(false);
  //const api_url = process.env.REACT_APP_API_URL;

  function hasToken() {
    const token = localStorage.getItem("token");
    const public_id = localStorage.getItem("public_id");

    if (token && public_id) {
      return true;
    } else {
      return false;
    }
  }

  function handleLogout() {
    deleteInfoInLocalStorage();
    setAuth(false);
  }

  function deleteInfoInLocalStorage() {
    localStorage.removeItem("token");
    localStorage.removeItem("public_id");
  }

  function isAuthenticated() {
    const token = localStorage.getItem("token");
    const public_id = localStorage.getItem("public_id");

    if (token && public_id) {
      return true;
    } else {
      return false;
    }
  }

  function saveLogin(values) {
    localStorage.setItem("token", values["token"]);
    localStorage.setItem("public_id", values["public_id"]);
    setAuth(true);
  }

  return (
    <AuthContext.Provider
      value={{
        isAuth,
        setAuth,
        saveLogin,
        isAuthenticated,
        handleLogout,
        hasToken,
      }}
    >
      {children}
    </AuthContext.Provider>
  );
};

export default AuthContext;
