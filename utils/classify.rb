#!/usr/bin/env ruby
# This script magically converts a struct definition to a QObject class definition.
#
# Example usage:
# ruby classify.rb <<< "struct Person { QString name; int age; }"

require "erb"

struct_name_regexp = /struct\s+(\S+)/
struct_member_regexp = /\s*(\S+)\s+(\S+)\s*;/

doc = STDIN.read

struct_name = doc.scan(struct_name_regexp).flatten.first
members = doc.scan(struct_member_regexp)

class String; def upcasefirst; self[0].upcase + self[1..-1]; end; end

puts ERB.new(DATA.read).result binding

__END__
class <%= struct_name %> : public QObject
{
    Q_OBJECT
<% members.each do |m| %>
    Q_PROPERTY(<%= m[0] %> <%= m[1] %> READ <%= m[1] %> WRITE set<%= m[1].upcasefirst %>)<% end %>

public:
<% members.each do |m| %>
    <%= m[0] %> <%= m[1] %>() const { return _<%= m[1] %>; }
    void set<%= m[1].upcasefirst %>(<%= m[0] %> <%= m[1] %>) { _<%= m[1] %> = <%= m[1] %>; }<% end %>

private:
<% members.each do |m| %>
    <%= m[0] %> _<%= m[1] %>;<% end %>
};
