import { createContext, useState } from "react";

const AuthContext = createContext({});

export const AuthProvider = ({ children }) => {
  const [isAuth, setAuth] = useState(false);

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
    localStorage.removeItem('pageNumber');
    localStorage.removeItem('pageSize');
    localStorage.removeItem('Copied');
    localStorage.removeItem("User");
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
    localStorage.setItem("token", values.user.token);
    localStorage.setItem("public_id", values.user.user["public-id"]);
    localStorage.setItem('User', JSON.stringify(values.user.user));
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
