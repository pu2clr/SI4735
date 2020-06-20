/*
Version 1.0.1

Copyright 2014 Red White Silver GmbH

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

(function($, window, document, undefined) {

	var pluginName = 'rotaryswitch',
		defaults = {
			minimum: 0, // Minimal value
			maximum: 30, // Maximum value
			step: 1, // Step size
			snapInMotion: true, // Snap to steps in motion
			beginDeg: 0, // Start point in deg
			lengthDeg: 360,	// Length in deg
			minimumOverMaximum: true, // Which value will used, if the the start and the end point at the same deg.
			showInput: false, // Show input element
			showMarks: true, // Show deg marks
			themeClass: 'defaultTheme' // Theme class
		};
	
	function Plugin(element, options) {
		this.element = $(element);
		this.domElements = {};
		this.htmlStructure = {
			wrap: '<div class="rotaryswitchPlugin"></div>',
			switchButton:  '<div class="switch"></div>',
			overlay: '<div class="overlay"></div>',
			marks: '<div class="marks"></div>',
			mark: '<div class="mark"></div>'
		};
		this.mousePosition = {x: -1, y: -1};
		this.switchDeg = 0;
		this.valueInPercent = 0;
		this.value = 0;
		this.steps = 0;
		this.totalDeg = 0;
		this.degPerStep = 0;
		this.lastTriggeredValue = -1;
		
		this.options = $.extend({}, defaults, options);
		this._defaults = defaults;
		this._name = pluginName;
		
		this.initialize();
	}

	Plugin.prototype = {
		
		/**
		* Initialze this plugin
		* See inline comments for more details
		*
		* @param	none
		* @return	none
		*/
		
		initialize: function() {
			
			// Save the needed jquery DOM elements
			this.domElements = {
				// Wrap the input element and save the parent as main element and add theme class:
				main: this.element.wrap(this.htmlStructure.wrap).parent().addClass(this.options.themeClass),
				switchButton: $(this.htmlStructure.switchButton),
				overlay: $(this.htmlStructure.overlay)
			};
			
			// Append addional dom elements:
			this.domElements.main.append([this.domElements.switchButton, this.domElements.overlay]);			
			
			// Calculate the length (Maximum - minimum)
			this.steps = Math.abs(this.options.maximum - this.options.minimum);
			
			// Calculate the total deg length
			this.totalDeg = this.options.lengthDeg;
			
			// Calculate deg per step
			this.degPerStep = this.totalDeg / this.steps;
			
			// Listen to some necessary events 
			this.domElements.main.on('mousedown', $.proxy(this.onMouseDown, this));
			this.domElements.main.on('touchstart', $.proxy(this.onTouchStart, this));
			this.element.on('change', $.proxy(this.onChangeElementValue, this));
			
			this.readValueFromInput(); // Get the value from the input element
			
			this.rotateSwitch(); // Rotate the switch
		
			// Show marks if wished
			if (this.options.showMarks === true) {
				this.renderMarks();
			}
			
			// Show the input element if wished
			if (this.options.showInput === false) {
				this.element.hide();
			}
		},
		
		/**
		* Adds marks for each step
		*
		* @param	none
		* @return	none
		*/
		
		renderMarks: function() {
			var i=0,
				len = this.steps / this.options.step,
				deg = this.options.beginDeg,
				degPerStep = this.degPerStep * this.options.step,
				marks = $(this.htmlStructure.marks);
			
			for (; i < len; i += 1) {
				deg += degPerStep;
				var mark = $(this.htmlStructure.mark).css({'transform': 'rotate('+deg+'deg) translate(0, -'+ (this.domElements.main.width()/2 + (this.domElements.main.width()*0.1)) +'px)'});
				marks.append(mark);
			}
						
			this.domElements.main.append(marks);
		},
		
		
		/**
		* On mouse down event handler
		* Save the mouse position (x, y) to the object this.mousePosition
		* call the method this.startHandling 
		*
		* @param	event	jquery mouse event
		* @return	none
		*/
		onMouseDown: function(event) {
			event.preventDefault();
			
			this.mousePosition.x = event.pageX;
			this.mousePosition.y = event.pageY;
			
			this.startHandling();
		},
		
		/**
		* On mouse up event handler
		* call the method this.stopHandling 
		*
		* @param	event	jquery mouse event
		* @return	none
		*/
		onMouseUp: function(event) {
			this.stopHandling();
		},
		
		/**
		* On mouse move event handler
		* Save the mouse position (x, y) to the object this.mousePosition
		* call the method this.calculateSwitchDeg 
		* call the method this.calculateValueByDeg
		* call the method this.rotateSwitch
		* call the method this.setValueToInput
		*
		* @param 	event	jquery mouse event
		* @return	none
		*/		
		onMousemove: function(event) {
			event.preventDefault();
			this.mousePosition.x = event.pageX;
			this.mousePosition.y = event.pageY;
			this.calculateSwitchDeg();
			this.calculateValueByDeg();
			this.rotateSwitch();
			this.setValueToInput();
		},		

		/**
		* On touch start event handler
		* Identical width this.onMouseDown
		*
		* @param	event	jquery touch event
		* @return 	none
		* @see		this.onMouseDown
		*/
		onTouchStart: function(event) {
			event.preventDefault();
			this.mousePosition.x = event.originalEvent.targetTouches[0].pageX;
			this.mousePosition.y = event.originalEvent.targetTouches[0].pageY;
			this.startHandling();
		},
		
		/**
		* On touch end event handler
		* Identical width this.onMouseUp
		*
		* @param	event	jquery touch event
		* @return 	none
		* @see		this.onMouseUp
		*/
		onTouchEnd: function(event) {
			this.stopHandling();
		},
		
		/**
		* On touch move event handler
		* Identical width this.onMousemove
		*
		* @param	event	jquery touch event
		* @return 	none
		* @see		this.onMousemove
		*/	
		onTouchMove: function(event) {
			event.preventDefault();
			
			this.mousePosition.x = event.originalEvent.targetTouches[0].pageX;
			this.mousePosition.y = event.originalEvent.targetTouches[0].pageY;
			
			this.calculateSwitchDeg();
			this.calculateValueByDeg();
			this.rotateSwitch();
			this.setValueToInput();
		},
		
		/**
		* On change input element event handler
		* If event not triggered by plugin:
		* Call the method this.readValueFromInput
		* Call the method this.rotateSwitch
		*
		* @param	event	jquery event
		* @return 	none
		*/
		onChangeElementValue: function(event) {
			if (!event.plugin || event.plugin !== this) {
				this.readValueFromInput();
				this.rotateSwitch();
			}
		},
		
		/**
		* Triggered by mouse or touch begin event
		* Calls some methods
		* Listen to some events
		* Add class 'active' to the main DOM element (this.domElements.main)
		*
		* @param	none
		* @return 	none
		*/
		startHandling: function() {
			this.calculateSwitchDeg();
			this.calculateValueByDeg();
			this.rotateSwitch();
			this.setValueToInput();
		
			$(document).on('mouseup', $.proxy(this.onMouseUp, this));
			$(document).on('mousemove', $.proxy(this.onMousemove, this));
			$(document).on('touchend', $.proxy(this.onTouchEnd, this));
			$(document).on('touchmove', $.proxy(this.onTouchMove, this));
			
			this.domElements.main.addClass('active');
		},
		
		/**
		* Triggered by mouseup or touchend event
		* Stop listen to some events
		* Call method this.rotateSwitch
		* Remove class 'active' from the main DOM element (this.domElements.main)
		*
		* @param	none
		* @return 	none
		*/
		stopHandling: function() {
			$(document).off('mouseup', $.proxy(this.onMouseUp, this))
				.off('mousemove', $.proxy(this.onMousemove, this))
				.off('touchend', $.proxy(this.onTouchEnd, this))
				.off('touchmove', $.proxy(this.onTouchMove, this));
			
			this.rotateSwitch(true);
			this.domElements.main.removeClass('active');
		},
		
		/**
		* Calculate the switch deg by the element position and the mouse position
		* Stores the switch deg in this.switchDeg
		* @param	none
		* @return 	none
		*/
		calculateSwitchDeg: function() {
			var offset =  this.domElements.main.offset(),
				radians = Math.atan2(this.mousePosition.x - (offset.left + (this.domElements.main.width()/2)), this.mousePosition.y - (offset.top + (this.domElements.main.height()/2)));
			
			if (this.mousePosition.x !== -1) {
				this.switchDeg = (radians * (180 / Math.PI) * -1) + 180;
			}
		},
		
		/**
		* Calculate the value by deg
		* Stores the in percent in this.valueInPercent
		* Stores the value in this.value
		* @param	none
		* @return 	none
		*/
		calculateValueByDeg: function() {
			var range = this.options.maximum - this.options.minimum;
			
			if (this.switchDeg - this.options.beginDeg > 0) {
				this.valueInPercent = (this.switchDeg - this.options.beginDeg)  / this.totalDeg;
			} else {
				this.valueInPercent = (this.switchDeg - this.options.beginDeg + 360)  / this.totalDeg;
			}
			
			if (this.valueInPercent > 1) {
				if (this.valueInPercent > (((360 / this.totalDeg)-1) / 2)+1 ) {
					this.valueInPercent = 0;
				} else {
					this.valueInPercent = 1;
				}
			}
			
			this.value = ~~ (((((range * this.valueInPercent) < 0) ? -0.5 : 0.5) + ((range * this.valueInPercent) / this.options.step))) * this.options.step;
			this.value += this.options.minimum;
			
			if (this.options.lengthDeg === 360 && (this.value === this.options.minimum || this.value === this.options.maximum)) {
				if (this.options.minimumOverMaximum === true) {
					this.value = this.options.minimum;
				} else {
					this.value = this.options.maximum;
				}
			}
		},
		
		/**
		* Rotate the switch with css transform
		* snap to the the next rounded value if the parameter snap is true
		* @param	snap	boolean
		* @return 	none
		*/
		rotateSwitch: function(snap) {
			var deg = 0,
				exactDeg = (this.valueInPercent * this.totalDeg),
				roundedDeg = ((this.value / this.steps) * this.totalDeg) - (this.options.minimum * this.degPerStep),
				difference = Math.abs(Math.abs(exactDeg) - Math.abs(roundedDeg)),
				rotateString = '';
			
			if (snap === true || (this.options.snapInMotion === true && difference < this.degPerStep / 6)) {
				if (roundedDeg + this.options.beginDeg < 360) {
					deg = (roundedDeg + this.options.beginDeg);
				} else {
					deg = roundedDeg + this.options.beginDeg - 360;
				}
			} else {
				if (exactDeg + this.options.beginDeg < 360) {
					deg = (exactDeg + this.options.beginDeg);
				} else {
					deg = exactDeg + this.options.beginDeg - 360;
				}
			}

			rotateString = ['rotate(', deg, 'deg)'].join('');
			this.domElements.switchButton.css({
				'transform': rotateString,
				'-webkit-transform': rotateString,
				'-moz-transform': rotateString,
				'-o-transform': rotateString,
				'-ms-transform': rotateString
			});
		},
		
		/**
		* Read the valur from the input element
		* If no value available, this.options.minimum is used
		* @param	snap	boolean
		* @return 	none
		*/
		readValueFromInput: function() {
			var elementValue = parseInt(this.element.val(), 10);
			if (isNaN(elementValue) === true) {
				this.value = this.options.minimum;
			} else {
				this.value = Math.max(this.options.minimum, elementValue);
				this.value = Math.min(this.options.maximum, this.value);
			}
			
			this.value -= this.options.minimum;
			this.valueInPercent = this.value / (this.options.maximum - this.options.minimum); 
		},
		
		/**
		* Set the value to the input element
		* and trigger the change event on the input element
		* @param	none
		* @return 	none
		*/
		setValueToInput: function() {
			if (this.value !== this.lastTriggeredValue) {
				this.lastTriggeredValue = this.value;
				this.element.val(this.value).trigger({type: 'change', plugin: this});	
			}
		}
	};
	
	$.fn[pluginName] = function(options) {
		return this.each(function() {
			if (!$.data(this, 'plugin_' + pluginName)){
				$.data(this, 'plugin_' + pluginName, new Plugin(this, options));
			}
		});
	};
	
}(jQuery, window, document));