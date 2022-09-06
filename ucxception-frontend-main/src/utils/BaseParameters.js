import CreateFields from "./CreateFields";

export default function BaseParameters(parameter, key, index) {
  function ParameterString() {
    return CreateFields().createInputText(
      index,
      "8",
      key,
      key,
      key,
      parameter["require"],
      "condition" in parameter
        ? "minLength" in parameter["condition"]
          ? parameter["condition"]["minLength"]
          : ""
        : "",
      "condition" in parameter
        ? "maxLength" in parameter["condition"]
          ? parameter["condition"]["maxLength"]
          : ""
        : "",
      "default" in parameter ? parameter["default"] : "",
      "placeholder" in parameter ? parameter["placeholder"] : "",
      null,
      parameter["required"],
      "The field must be filled in!"
    );
  }

  function ParameterInteger() {
    return CreateFields().createInputNumber(
      index,
      "8",
      key,
      key,
      key,
      parameter["require"],
      "condition" in parameter
        ? "step" in parameter["condition"]
          ? parameter["condition"]["step"]
          : ""
        : 1,
      "condition" in parameter
        ? "minValue" in parameter["condition"]
          ? parameter["condition"]["minValue"]
          : ""
        : "",
      "condition" in parameter
        ? "maxValue" in parameter["condition"]
          ? parameter["condition"]["maxValue"]
          : ""
        : "",
      "default" in parameter ? parameter["default"] : "",
      "placeholder" in parameter ? parameter["placeholder"] : "",
      null,
      parameter["required"],
      "The field must be filled in!"
    );
  }

  function ParameterBolean() {
    return CreateFields().createInputRadio(
      index,
      "8",
      key,
      key,
      key,
      null,
      null
    );
  }

  function ParameterMultiple(value) {
    return CreateFields().createMultipleDropdown(index, "10", key, key, value);
  }

  function ParameterSingle(label, require, value) {
    return CreateFields().createSelect(
      index,
      "10",
      label,
      key,
      key,
      require,
      null,
      value
    );
  }

  function ParameterTarget(require, display, value) {
    return CreateFields().createSelect(
      index,
      "10",
      display,
      key,
      key,
      require,
      null,
      value
    );
  }

  function ParameterFile(accept) {
    return CreateFields().createInputFile(index, "10", key, key, accept);
  }

  return {
    ParameterString,
    ParameterInteger,
    ParameterBolean,
    ParameterMultiple,
    ParameterSingle,
    ParameterTarget,
    ParameterFile,
  };
}
