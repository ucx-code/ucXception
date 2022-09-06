import React from "react";
import { CCol, CRow, CButton } from "@coreui/react";
import BaseParameters from "../../utils/BaseParameters";
import CreateFields from "../../utils/CreateFields";

const ComponentParameters = ({
  componentData,
  associatedComponents,
  hosts,
}) => {
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
          parameter["require"],
          parameter["values"]
        );
      case "file":
        return BaseParameters(parameter, key, index).ParameterFile(
          parameter["accept"]
        );
      default:
        return null;
    }
  };

  return (
    <>
      <CRow xs={{ gutterY: 3 }}>
        {componentData !== undefined && componentData !== null ? (
          <>
            {CreateFields().createInputText(
              101,
              "8",
              "Name",
              "name",
              "name",
              true,
              1,
              30,
              "",
              "",
              null,
              true,
              "Component must have a name!"
            )}
            {"target" in componentData ? (
              "require" in componentData["target"] ? (
                BaseParameters(
                  componentData["target"]["require"],
                  "target",
                  102
                ).ParameterTarget(
                  componentData["target"]["require"],
                  componentData["target"]["display"],
                  hosts
                )
              ) : (
                <CCol sm={12}>
                  <h6>Can not display target field.</h6>
                </CCol>
              )
            ) : (
              <CCol sm={12}>
                <h6>Component does not need to configure a target.</h6>
              </CCol>
            )}
          </>
        ) : null}
        {"components_allowed" in componentData &&
        associatedComponents !== undefined &&
        associatedComponents !== null
          ? BaseParameters(null, "associated_component", 0).ParameterSingle(
              "Associate component",
              true,
              associatedComponents
            )
          : null}
        {componentData !== undefined && componentData !== null ? (
          "parameters" in componentData &&
          Object.keys(componentData["parameters"]).length > 0 ? (
            Object.keys(componentData["parameters"]).map((key, index) =>
              verifyInputs(componentData["parameters"][key], key, index)
            )
          ) : (
            <CCol sm={12}>
              <h6>Component does not have configuration parameters.</h6>
            </CCol>
          )
        ) : (
          <CCol sm={12}>
            <h6>First need to choose a component.</h6>
          </CCol>
        )}
        {componentData !== undefined && componentData !== null ? (
          <CCol xs={12} className="text-center">
            <CButton color="primary" type="submit">
              Create
            </CButton>
          </CCol>
        ) : null}
      </CRow>
    </>
  );
};

export default ComponentParameters;
