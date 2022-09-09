import React from "react";
import { Route, Routes } from "react-router-dom";
import breadcrumbs_routes from "./routes";
//Pages
const DefaultLayout = React.lazy(() => import("./layout/DefaultLayout"));
const Register = React.lazy(() => import("./views/Register/Register"));
const Login = React.lazy(() => import("./views/Login/Login"));
const Reset = React.lazy(() => import("./views/Reset/Reset"));
const ResetConfirmation = React.lazy(() =>
  import("./views/Reset/ResetConfirmation")
);
const Menu = React.lazy(() => import("./views/Menu/Menu"));
const Statistics = React.lazy(() => import("./views/Display/Statistics"));
const Charts = React.lazy(() => import("./views/Display/Charts"));

const RoutePages = () => {
  return (
    <Routes>
      {/* public routes */}
      <Route path="/" exact element={<Login />} />
      <Route path="/login" element={<Login />} />
      <Route path="/register" element={<Register />} />
      <Route path="/reset" element={<Reset />} />
      <Route
        path="/reset_confirmation/:reset_token"
        element={<ResetConfirmation />}
      />

      {/*private routes */}
      <Route element={<DefaultLayout />}>
        <Route path="/menu" element={<Menu />} />
        <Route
          path="/campaign/:campaign_id/statistics"
          element={<Statistics />}
        />
        <Route path="/campaign/:campaign_id/charts" element={<Charts />} />
        {breadcrumbs_routes.map((breadcrumb_route, index) => {
          return (
            <Route
              key={index}
              path={breadcrumb_route.pathname}
              name={breadcrumb_route.name}
              element={breadcrumb_route.element}
            />
          );
        })}
      </Route>
    </Routes>
  );
};

export default RoutePages;
