import React from "react";
import { CCol, CRow } from "@coreui/react";
import BaseParameters from "../../utils/BaseParameters";

const CampaignParameters = ({ campaignData, campaignType }) => {
  //Verify if the parameter has the obligatory data
  const verifyInputs = (parameter, key, index) => {
    if ("type" in parameter && "require" in parameter) {
      return renderInputFields(parameter, key, index);
    }
  };

  //According to the type an input field is created
  const renderInputFields = (parameter, key, index) => {
    switch (parameter["type"]) {
      case "string":
        return BaseParameters(parameter, key, index).ParameterString();
      case "integer":
        return BaseParameters(parameter, key, index).ParameterInteger();
      case "boolean":
        return BaseParameters(parameter, key, index).ParameterBolean();
      case "multiple":
        return BaseParameters(parameter, key, index).ParameterMultiple(
          parameter["values"]
        );
      case "single":
        return BaseParameters(parameter, key, index).ParameterSingle(
          parameter["values"]
        );
      default:
        return null;
    }
  };

  return (
    <>
      <CRow xs={{ gutterY: 3 }}>
        {campaignType ? (
          campaignData !== undefined && campaignData !== null ? (
            campaignData.map((object, key) =>
              object["campaign_name"] === campaignType ? (
                "parameters" in object ? (
                  Object.keys(object["parameters"]).length > 0 ? (
                    Object.keys(object["parameters"]).map((key, index) =>
                      verifyInputs(object["parameters"][key], key, index)
                    )
                  ) : null
                ) : (
                  <CCol key={key} className="text-center" sm={12}>
                    <h6>Campaign does not have configuration parameters</h6>
                  </CCol>
                )
              ) : null
            )
          ) : null
        ) : (
          <CCol className="text-center" sm={12}>
            <h6>First need to choose a campaign type</h6>
          </CCol>
        )}
      </CRow>
    </>
  );
};

export default CampaignParameters;
