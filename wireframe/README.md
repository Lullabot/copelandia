#The Problem
Designing in the browser is becoming the standard approach for many designers today. While it has many advantages over static Photoshop comps, it is not without it's drawbacks.

* HTML is Complex
  *  Creating a contact form in Photoshop only requires a few rectangles and text blocks, but in HTML there is a considerable amount of markup needed to accurately portray the labels and fields of a form.
* Drupal Markup is Even More Complex
  *  Drupal (or any other CMS) will come with it's own conventions for forms, search fields, navigation and other UI elements. We often use generic markup to speed up our design process, but in the end this means a longer themeing process.
* Complex UI Elements Slow Down Design
  *  Creating a slideshow in Photoshop requires an image and a few arrows. Done.
  *  Creating a working slideshow in html/css/js takes considerably more time! And innevitably, after you spend a day creating a slideshow, the client will go "nah" and ask for a carousel instead.

##Project Goals
* Modular
  *  Easily add new components
  *  Easily override existing components
* Understandable
  *  Thorough Documentation
  *  Links to information on relevant subjects / tutorials
  *  Component preview pages
* Flexible
  *  Framework agnostic, yet capable (SUSY, ZURB Foundation, etc... or nothing at all)
  *  Easily adjust stylesheet usage for a given template or layout
  *  Write markup with HTML, HAML, <insert your favorite>
* Lightweight
  *  Nothing should ever be forced on the user, if desired, the ability to produce a simple HTML page with a single stylesheet should be available.
     <em>E.g: Do not compile that which is not wanted.</em>

#The Solution
Our solution is to look at HTML with the same module approach that we already use on CSS. By breaking up our pages into reusable partials, and putting those partials into preset column layout templates,  we are able to construct a new full featured page within minutes.

##Partials
These partials will include dozens of standard drupal components (forms, comment blocks, search fields, users online, breadcrumbs, pagers etc) that can be inserted into your template with a single line of code. Variables can also be passed into the partial to change the outcome of the partial (form-comment-number = 5 or slideshow-images-number = 3).

##Default Styles
By default, every page you create will be served with a default set of "wireframe quality" styles (think zurb light). These styles will give your site a general frame, typography and element layout. All of the modules will look presentable with appropriate padding, margins and light background shades.

##Wireframes in Minutes
Without writing a single line of CSS the designer will have a presentable wire frame full of standard drupal components, navigation, and even carousels and slide shows. Once a layout is chosen those default styles can be turned off sitewide or page by page and the designer can start doing what they do best.

##Serve Magic
This is all made possible by a ruby tool called Serve that serves up these pages dynamically without the need for compiling. It is also coupled with Sass and Compass, so with a single 'serve' command on the command line you have started a ruby server instance, and are compiling your Sass. Serve is compatible with ERB, Haml, Markdown and various other templating languages.

#Protobots

The Tools
<a href="http://www.get-serve.com">Get-Serve</a>

##The Team
* Carwin Young  - Front End - Project Co-Lead
* Micah Godbolt - Front End - Project Co-Lead
* Jared Ponchot - Design Tester


##Tips
The following render can be used to pass variables to the called partial.

    <%= render "/partials/block", :locals => { :title => "Past Events" } %>

The partial would then look like the following

    <%
     title ||= "Default Title"
    %>
    <h2><%= title %></h2>
