
const Numpad = {
    elements: {
        main: null,
        keysContainer: null,
        keys: []
    },

    eventHandlers: {
        oninput: null,
        onclose: null
    },

    properties: {
        value: "",
        capsLock: false
    },

    init() {
        // Create main elements
        this.elements.main = document.createElement("div");
        this.elements.keysContainer = document.createElement("div");

        // Setup main elements
        this.elements.main.classList.add("numpad", "numpad--hidden");
        this.elements.keysContainer.classList.add("numpad__keys");
        this.elements.keysContainer.appendChild(this._createKeys());

        this.elements.keys = this.elements.keysContainer.querySelectorAll(".numpad__key");

        // Add to DOM
        this.elements.main.appendChild(this.elements.keysContainer);
        document.body.appendChild(this.elements.main);

        // Automatically use numpad for elements with .use-numpad-input
        document.querySelectorAll(".use-numpad-input").forEach(element => {
            element.addEventListener("focus", () => {
                this.open(element.value, currentValue => {
                    element.value = currentValue;
                });
            });
        });
		
        document.querySelectorAll(".use-numpad-input-null").forEach(element => {
            element.addEventListener("focus", () => {
				element.value = ''
                this.open(element.value, currentValue => {
                    element.value = currentValue;
                });
            });
        });

        const concernedElementA = document.querySelector(".numpad")
        const concernedElementB = document.querySelector(".use-numpad-input")
        const concernedElementC = document.querySelector(".use-numpad-input-null")

        document.addEventListener("mousedown", (event) => {
            if (concernedElementA.contains(event.target)==0 && concernedElementB.contains(event.target)==0){
                this.close();
            }
        })

        document.addEventListener("mousedown", (event) => {
            if (concernedElementA.contains(event.target)==0 && concernedElementC.contains(event.target)==0){
                this.close();
            }
        })
    },

    _createKeys() {
        const fragment = document.createDocumentFragment();
        const keyLayout = [
            "1", "2", "3",
            "4", "5", "6",
            "7", "8", "9",
            "done", "0", "backspace"
        ];

        // Creates HTML for an icon
        const createIconHTML = (icon_name) => {
            return '<i class="material-icons">' + icon_name + '</i>';
        };

        keyLayout.forEach(key => {
            const keyElement = document.createElement("button");
            const insertLineBreak = ["9", "6", "3"].indexOf(key) !== -1;

            // Add attributes/classes
            keyElement.setAttribute("type", "button");
            keyElement.classList.add("numpad__key");

            switch (key) {
                case "backspace":
                    //keyElement.classList.add("numpad__key--wide");
                    keyElement.innerHTML = "⌫";//createIconHTML("backspace");

                    keyElement.addEventListener("click", () => {
                        this.properties.value = this.properties.value.substring(0, this.properties.value.length - 1);
                        this._triggerEvent("oninput");
                    });

                    break;

                case "done":
                    keyElement.classList.add("numpad__key--dark");
                    keyElement.innerHTML = "ˬ";//createIconHTML("check_circle");

                    keyElement.addEventListener("click", () => {
                        this.close();
                        this._triggerEvent("onclose");
                    });

                    break;

                default:
                    keyElement.textContent = key.toLowerCase();

                    keyElement.addEventListener("click", () => {
                        this.properties.value += this.properties.capsLock ? key.toUpperCase() : key.toLowerCase();
                        this._triggerEvent("oninput");
                    });

                    break;
            }

            fragment.appendChild(keyElement);

            if (insertLineBreak) {
                fragment.appendChild(document.createElement("br"));
            }
        });

        return fragment;
    },

    _triggerEvent(handlerName) {
        if (typeof this.eventHandlers[handlerName] == "function") {
            this.eventHandlers[handlerName](this.properties.value);
        }
    },

    _toggleCapsLock() {
        this.properties.capsLock = !this.properties.capsLock;

        for (const key of this.elements.keys) {
            if (key.childElementCount === 0) {
                key.textContent = this.properties.capsLock ? key.textContent.toUpperCase() : key.textContent.toLowerCase();
            }
        }
    },

    open(initialValue, oninput, onclose) {
        this.properties.value = initialValue || "";
        this.eventHandlers.oninput = oninput;
        this.eventHandlers.onclose = onclose;
        this.elements.main.classList.remove("numpad--hidden");
    },

    close() {
        this.properties.value = "";
        this.eventHandlers.oninput = oninput;
        this.eventHandlers.onclose = onclose;
        this.elements.main.classList.add("numpad--hidden");
    }
};

window.addEventListener("DOMContentLoaded", function () {
    Numpad.init();
});
