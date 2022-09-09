import React from "react";

const CampaignSetup = React.lazy(() =>
  import("./views/Campaign/CampaignSetup")
);
const CampaignExecution = React.lazy(() =>
  import("./views/Campaign/CampaignExecution")
);
const CampaignHosts = React.lazy(() =>
  import("./views/Campaign/CampaignHosts")
);
const ComponentPage = React.lazy(() =>
  import("./views/Components/ComponentPage")
);

const routes = [
  {
    pathname: "/campaign/setup",
    name: "Campaign",
    exact: true,
    element: <CampaignSetup />,
  },
  {
    pathname: "/campaign/executions",
    name: "Executions",
    element: <CampaignExecution />,
  },
  {
    pathname: "/campaign/hosts",
    name: "Hosts",
    element: <CampaignHosts />,
  },
  {
    pathname: "/preprobes",
    name: "PreProbes",
    element: <ComponentPage />,
  },
  {
    pathname: "/postprobes",
    name: "PostProbes",
    element: <ComponentPage />,
  },
  {
    pathname: "/parsers",
    name: "Parsers",
    element: <ComponentPage />,
  },
  {
    pathname: "/validators",
    name: "Validators",
    element: <ComponentPage />,
  },
  {
    pathname: "/transformers",
    name: "Transformers",
    element: <ComponentPage />,
  },
];

export default routes;
